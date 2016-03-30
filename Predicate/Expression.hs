module Expression where

import Data.List
import Data.Char
import Text.Parsec
import Control.Monad.State


import qualified Data.Set as S

data Expr = EOperation EOp Expr Expr
          | Not Expr
          | Quant Quantifier VName Expr
          | Term :=: Term
          | Predicate VName [Term]
    deriving (Eq, Ord)

data Term = TOperation TOp Term Term
		  | Function VName [Term]
		  | Succ Term
		  | Zero
		  | Variable VName
  	deriving (Eq, Ord)

type VName = String

data EOp = Impl | Disj | Conj deriving (Eq, Ord)

data TOp = Sum | Product deriving (Eq, Ord)

data Quantifier = QAll | QAny deriving (Eq, Ord)

instance Show EOp where
    show Impl = "->"
    show Disj = "|"
    show Conj = "&"

instance Show Quantifier where
    show QAll = "@"
    show QAny = "?"

instance Show TOp where
    show Sum = "+"
    show Product = "*"

instance Show Expr where
    show (EOperation op l r)        = bounded l ++ show op ++ bounded r
    show (Not e@(EOperation _ _ _)) = '!' : bounded e
    show (Not e)                    = '!' : show e
    show (Quant q var e)            = show q ++ var ++ bounded e
    show (t1 :=: t2)                = show t1 ++ '=' : show t2
    show (Predicate name terms)     = name ++ if (length terms /= 0) then bounds (intercalate "," (map show terms)) else ""

instance Show Term where
    show (TOperation op l r)         = bounded l ++ show op ++ bounded r
    show Zero                        = "0"
    show (Succ e@(TOperation _ _ _)) = bounded e ++ "'"
    show (Succ e)                    = show e ++ "'"
    show (Variable str)              = str
    show (Function name terms)       = name ++ bounds (intercalate "," (map show terms))

bounded :: Show a => a -> String
bounded = bounds . show

bounds :: String -> String
bounds s = "(" ++ s ++ ")"

infixr 4 \->
infixl 5 \|
infixl 6 \&

infixl 7 :=:

infixl 8 \*
infixl 9 \+

(\->) = EOperation Impl
(\|)  = EOperation Disj
(\&)  = EOperation Conj

(\+) = TOperation Sum
(\*) = TOperation Product


parseExpr :: String -> Either ParseError Expr
parseExpr = parse implication ""

implication :: Parsec String () Expr
implication = disjunction `chainr1` (token' "->" (EOperation Impl)) where
    disjunction = conjunctuion `chainl1` (try $ do{t <- token' "|" (EOperation Disj); notFollowedBy $ string "-"; return t})
    conjunctuion = negation `chainl1` (token' "&" (EOperation Conj))
    negation = do
                string "!"
                e <- negation
                return (Not e)
           <|> do
                q <- (token' "@" QAll) <|> (token' "?" QAny)
                var <- varname
                e <- negation
                return (Quant q var e)
           <|> try (do
                e <- betweenBracket implication
                return e)
           <|> do
                c <- satisfy isUpper
                cs <- many $ satisfy isDigit
                terms <- option [] (betweenBracket $ sepBy1 add (string ","))
                return (Predicate (c : cs) terms)
           <|> do
                t1 <- add
                string "="
                t2 <- add
                return (t1 :=: t2)
    varname = do
                c <- satisfy isLower
                cs <- many $ satisfy isDigit
                return (c : cs)
    add = chainl1 mult (token' "+" (TOperation Sum))
    mult = chainl1 succ (token' "*" (TOperation Product))
    succ = do
            e <- single
            succs <- many $ string "'"
            return $ (iterate Succ e) !! (length succs)
    single   =  token' "0" Zero
            <|> betweenBracket add
            <|> try (do
                        name <- varname
                        args <- betweenBracket $ sepBy1 add (string ",")
                        return (Function name args)
                    )
            <|> do
                name <- varname
                return $ Variable name

    betweenBracket = between (string "(") (string ")")
    token' str x = do {string str <?> "token<" ++ str ++ ">"; return x}


substituteTerm :: Expr -> VName -> Term -> Expr

substituteTerm (EOperation op a b) vname term = EOperation op (substituteTerm a vname term) (substituteTerm b vname term)
substituteTerm e@(Quant q vx expr) vname term = if (vx /= vname) then e else Quant q vx (substituteTerm expr vname term)
substituteTerm (Not e)             vname term = Not $ substituteTerm e vname term
substituteTerm (Predicate vx ts)   vname term = Predicate vx (map (\t -> substituteTerm' t vname term) ts)
substituteTerm (a :=: b)           vname term = (substituteTerm' a vname term) :=: (substituteTerm' b vname term)

substituteTerm' :: Term -> VName -> Term -> Term

substituteTerm' (TOperation op a b) vname term = TOperation op (substituteTerm' a vname term) (substituteTerm' b vname term)
substituteTerm' (Succ t)            vname term = Succ $ substituteTerm' t vname term
substituteTerm' Zero _ _ = Zero
substituteTerm' v@(Variable vx)     vname term = if (vx /= vname) then v else term
substituteTerm' (Function vx ts)    vname term = Function vx (map (\t -> substituteTerm' t vname term) ts)


data MatchResult a = AnyRes         -- any result allows
                   | Res a          -- allowed result
                   | FailRes a      -- matched result, but not allowed
                   | MatchError     -- no matched result
               deriving Show

(<++>) :: Eq a => MatchResult a -> MatchResult a -> MatchResult a
AnyRes <++> smth = smth
smth <++> AnyRes = smth
(Res x) <++> (Res y) | x == y    = Res x
                     | otherwise = MatchError
(FailRes x) <++> (Res y) | x == y    = FailRes x
                         | otherwise = MatchError
(Res x) <++> (FailRes y) | x == y    = FailRes x
                         | otherwise = MatchError
_ <++> _ = MatchError


toExpr :: Term -> Expr
toExpr e = Predicate "TERM" [e]

matchByTerm :: Expr -> Expr -> VName -> MatchResult Term

matchByTerm (EOperation op a b) (EOperation op' a' b') vname | op == op' = (matchByTerm a a' vname) <++> (matchByTerm b b' vname)
                                                             | otherwise = MatchError
matchByTerm (Quant q vx expr) (Quant q' vx' expr') vname | q /= q' || vx /= vx' = MatchError
                                                         | vx == vname          = if (expr == expr') then AnyRes else MatchError
                                                         | otherwise            = let res = matchByTerm expr expr' vname in
                                                                                    case res of
                                                                                    Res r -> if S.member vx' (freeVariables . toExpr $ r)
                                                                                                then FailRes r
                                                                                                else Res r
                                                                                    a -> a
matchByTerm (Predicate vx ts) (Predicate vx' ts') vname | vx /= vx' = MatchError
                                                        | otherwise = foldr (\(x, x') res -> (matchByTerm' x x' vname) <++> res) AnyRes (zip ts ts')
matchByTerm (a :=: b) (a' :=: b') vname = (matchByTerm' a a' vname) <++> (matchByTerm' b b' vname)
matchByTerm (Not e)   (Not e')    vname = matchByTerm e e' vname
matchByTerm _ _ _ = MatchError

matchByTerm' :: Term -> Term -> VName -> MatchResult Term

matchByTerm' (TOperation op a b) (TOperation op' a' b') vname | op == op' = (matchByTerm' a a' vname) <++> (matchByTerm' b b' vname)
                                                              | otherwise = MatchError
matchByTerm' (Function vx ts) (Function vx' ts') vname | vx /= vx' = MatchError
                                                       | otherwise = foldr (\(x, x') res -> (matchByTerm' x x' vname) <++> res) AnyRes (zip ts ts')
matchByTerm' (Succ t) (Succ t') vname = matchByTerm' t t' vname
matchByTerm' Zero Zero vname = AnyRes

matchByTerm' v@(Variable vx) term vname | vx == vname = Res term
                                        | v == term   = AnyRes
                                        | otherwise   = MatchError
matchByTerm' _ _ _ = MatchError

freeVariables :: Expr -> S.Set VName
freeVariables e = execState (freeVariablesImpl e) S.empty
    where
    freeVariablesImpl (EOperation _ a b) = do {freeVariablesImpl a; freeVariablesImpl b}
    freeVariablesImpl (Quant _ vname e) = do {has <- gets (S.member vname); freeVariablesImpl e; if has then return () else modify (S.delete vname)}
    freeVariablesImpl (Predicate _ ts) = freeVariablesList ts
    freeVariablesImpl (Not e) = freeVariablesImpl e
    freeVariablesImpl (t1 :=: t2) = do {freeVariablesImpl' t1; freeVariablesImpl' t2}

    freeVariablesList (x:xs) = do {freeVariablesImpl' x; freeVariablesList xs}
    freeVariablesList []     = return ()

    freeVariablesImpl' (TOperation _ a b) = do {freeVariablesImpl' a; freeVariablesImpl' b}
    freeVariablesImpl' (Function _ ts) =  freeVariablesList ts
    freeVariablesImpl' (Succ e) = freeVariablesImpl' e
    freeVariablesImpl' Zero = return ()
    freeVariablesImpl' (Variable vname) = modify (S.insert vname)

nonFreeVariables :: Expr -> S.Set VName
nonFreeVariables e = execState (nonFreeVariablesImpl e) S.empty
    where
    nonFreeVariablesImpl (EOperation _ a b) = do {nonFreeVariablesImpl a; nonFreeVariablesImpl b}
    nonFreeVariablesImpl (Quant _ vname e) = do {modify (S.insert vname); nonFreeVariablesImpl e;}
    nonFreeVariablesImpl (Not e) = nonFreeVariablesImpl e
    nonFreeVariablesImpl _ = return ()