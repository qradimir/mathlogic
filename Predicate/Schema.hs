module Schema where

import Expression
import Control.Monad.State
import Control.Monad.Except

import qualified Data.Map as M

data SExpr = SOperation EOp ExecOrder SExpr SExpr
           | SNot SExpr
           | SQuant Quantifier VName SExpr
           | Reference VName
           | ESubst VName VName Term
           | SSubst VName VName
    deriving (Eq, Show)

data MatchExc = Mismatched | BadSchema | InvalidSubst Expr Term VName

instance Show MatchExc where
    show Mismatched = "Данное выражения не подходит под схему"
    show BadSchema = "Неверная схема"
    show (InvalidSubst e t x) = "Терм " ++ show t ++ " не свободен для подстановки в " ++ show e ++ " вместо переменной " ++ x


data ExecOrder = NatOrder | RevOrder
    deriving (Eq, Show)

type MatchOutput = (M.Map VName VName, M.Map VName Expr)

doInOrder :: Monad m => ExecOrder -> m a -> m a -> m a
doInOrder NatOrder a b = do {a; b}
doInOrder RevOrder a b = do {b; a}


subtituteSchema :: SExpr -> MatchOutput -> Expr
subtituteSchema s (vars, exprs) = subtituteSchemaImpl s where
    subtituteSchemaImpl (SOperation e _ sa sb) = EOperation e (subtituteSchemaImpl sa) (subtituteSchemaImpl sb)
    subtituteSchemaImpl (SQuant q x se)        = Quant q (vars M.! x) (subtituteSchemaImpl se)
    subtituteSchemaImpl (SNot se)              = Not (subtituteSchemaImpl se)
    subtituteSchemaImpl (Reference r)          = exprs M.! r
    subtituteSchemaImpl (ESubst r x t)         = let x' = (vars M.! x) in substituteTerm (exprs M.! r) x' (substituteTerm' t x $ Variable x')
    subtituteSchemaImpl (SSubst r x)           = error "invalid substitution"

matchSchema :: SExpr -> Expr -> Either MatchExc MatchOutput
matchSchema s e = runExcept . execStateT (matchSchemaImpl s e) $ (M.empty, M.empty) where
    matchSchemaImpl :: SExpr -> Expr -> StateT MatchOutput (Except MatchExc) ()
    matchSchemaImpl (SOperation se order sa sb) (EOperation e a b) | se /= e   = throwError Mismatched
                                                                   | otherwise = doInOrder order
                                                                                    (matchSchemaImpl sa a)
                                                                                    (matchSchemaImpl sb b)
    matchSchemaImpl (SNot se) (Not e) = matchSchemaImpl se e
    matchSchemaImpl (Reference ref) expr = do
                                          state <- get
                                          let e = M.lookup ref (snd state)
                                          case e of
                                                Nothing -> modifySnd $ (M.insert ref expr)
                                                Just expr' -> if (expr == expr') then return ()
                                                                                 else throwError Mismatched
    matchSchemaImpl (SQuant sq sx se) (Quant q x e) | sq /= q = throwError Mismatched
                                                    | otherwise = do
                                                                 state <- get
                                                                 let sxname = M.lookup sx (fst state)
                                                                 case sxname of
                                                                        Nothing -> do
                                                                                    modifyFst $ (M.insert sx x)
                                                                                    matchSchemaImpl se e
                                                                        Just x' -> if (x' == x) then matchSchemaImpl se e
                                                                                                else throwError Mismatched
    matchSchemaImpl (SSubst ref sx) expr = do
                                        state <- get
                                        let e = M.lookup ref (snd state)
                                        let sxname = M.lookup sx (fst state)
                                        case e of
                                            Nothing -> throwError BadSchema
                                            Just expr' -> (case sxname of
                                                                Nothing -> throwError BadSchema
                                                                Just x' -> do
                                                                            (case matchByTerm expr' expr x' of
                                                                                Res _ -> return ()
                                                                                FailRes fterm -> throwError $ InvalidSubst expr' fterm x'
                                                                                AnyRes -> return ()
                                                                                MatchError -> throwError Mismatched))
    matchSchemaImpl (ESubst ref sx term) expr = do
                                        state <- get
                                        let e = M.lookup ref (snd state)
                                        let sxname = M.lookup sx (fst state)
                                        case e of
                                            Nothing -> throwError BadSchema
                                            Just expr' -> (case sxname of
                                                                Nothing -> throwError BadSchema
                                                                Just x' -> do
                                                                            let realterm = substituteTerm' term sx (Variable x')
                                                                            let substed = substituteTerm expr' x' realterm
                                                                            if (substed == expr)
                                                                                then return ()
                                                                                else throwError Mismatched)
    matchSchemaImpl _ _ = throwError Mismatched

    modifyFst f = modify $ (\(x, y) -> (f x, y))
    modifySnd f = modify $ (\(x, y) -> (x, f y))


ref = Reference

opNat op = SOperation op NatOrder
opRev op = SOperation op RevOrder
quAny = SQuant QAny
quAll = SQuant QAll

infixr 5 /->
infixl 6 /|
infixl 7 /&

infixr 5 //->
infixl 6 //|
infixl 7 //&

a /-> b  = opNat Impl a b
a //-> b = opRev Impl a b

a /| b  = opNat Disj a b
a //| b = opRev Disj a b

a /& b  = opNat Conj a b
a //& b = opRev Conj a b

rA = ref "A"
rB = ref "B"
rC = ref "C"

ssubst_xa = SSubst "A" "x"
esubst_xa = ESubst "A" "x"

schema = [ rA /-> rB /-> rA                                     -- A->B->A
         , (rA /-> rB) /-> (rA /-> rB /-> rC) /-> (rA /-> rC)   -- (A->B)->(A->B->C)->(A->C)
         , rA /-> rB /-> rA /& rB                               -- A->B->A&B
         , rA /& rB /-> rA                                      -- A&B->A
         , rA /& rB /-> rB                                      -- A&B->B
         , rA /-> rA /| rB                                      -- A->A|B
         , rB /-> rA /| rB                                      -- B->A|B
         , (rA /-> rC) /-> (rB /-> rC) /-> (rA /| rB /-> rC)    -- (A->C)->(B->C)->(A|B->C)
         , (rA /-> rB) /-> (rA /-> SNot rB) /-> SNot rA         -- (A->B)->(A->!B)->!A
         , (SNot $ SNot rA) /-> rA                              -- !!A->A
         ]

qall_schema = "x" `quAll` rA /-> ssubst_xa      -- @xA->A[x:=T]
qany_schema = ssubst_xa //-> "x" `quAny` rA     -- A[x:=T]->?xA
induction   = esubst_xa Zero //& "x" `quAll` (rA /-> esubst_xa (Succ $ Variable "x")) //-> rA --A[x:=0]&@x(A->A[x:=x'])->A

qall_rule = rA /-> "x" `quAll` rB
qany_rule = "x" `quAny` rB /-> rA

vA = Variable "a"
vB = Variable "b"
vC = Variable "c"

sA = Succ vA
sB = Succ vB

axioma = [ vA :=: vB \-> sA :=: sB
         , vA :=: vB \-> vA :=: vC \-> vB :=: vC
         , sA :=: sB \-> vA :=: vB
         , Not (sA :=: Zero)
         , vA \+ sB :=: Succ (vA \+ vB)
         , vA \+ Zero :=: vA
         , vA \* Zero :=: Zero
         , vA \* sB :=: vA \* vB \+ vA
         ]

data AxiomaMatchResult = MatchedAxiom
                       | FailMatchedAxiom MatchExc
                       | NoMatches
    deriving Show


(><) :: SExpr -> Expr -> Bool
s >< e | (Right _) <- matchRes = True
       | (Left _) <- matchRes = False
       where matchRes = matchSchema s e

(!><) :: SExpr -> Expr -> AxiomaMatchResult
s !>< e | (Right _) <- matchRes                     = MatchedAxiom
        | (Left e@(InvalidSubst _ _ _)) <- matchRes = FailMatchedAxiom e
        | (Left Mismatched) <- matchRes             = NoMatches
       where matchRes = matchSchema s e

