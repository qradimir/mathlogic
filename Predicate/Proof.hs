module Proof where

import Expression
import Text.Parsec

import Schema


import Control.Monad.State
import Control.Monad.Except


import qualified Data.Set as S
import qualified Data.List as L
import qualified Data.Map as M

data Annotation = Axiom
                | Suppose
                | ModusPonens Expr
                | ExQuantRule VName Expr Expr
                | UnQuantRule VName Expr Expr

data AnnExpr = AnnExpr Expr Annotation

data Proof = Proof [Expr] Expr [Expr]
data AnnProof = AnnProof [Expr] Expr [AnnExpr]

data ProofError = FreeVariableForbiden VName Expr
                | InvalidTermSubtition Term VName Expr
                | SupposeVariableUsing VName Expr
                | Unproven

instance Show Annotation where
    show Axiom = "axiom"
    show Suppose = "assumption" -- or suppose?!
    show (ModusPonens _) = "Modus Ponens"
    show (ExQuantRule _ _ _) = "exist quantification"
    show (UnQuantRule _ _ _) = "unify quantification"

instance Show AnnExpr where
    show (AnnExpr e a) = show e ++ " : " ++ show a

instance Show Proof where
    show (Proof s e p) = (L.intercalate "," $ map show s) ++
                         "|-" ++ show e ++ "\n" ++
                         (L.intercalate "\n" $ map show p)

instance Show AnnProof where
    show (AnnProof s e p) = (L.intercalate "," $ map show s) ++
                            "|-" ++ show e ++ "\n" ++
                            (L.intercalate "\n" $ map show p)

instance Show ProofError where
    show (FreeVariableForbiden v e)   = ": переменная " ++ v ++ " входит свободно в формулу " ++ show e
    show (InvalidTermSubtition t v e) = ": терм " ++ show t ++ " не свободен для подстановки в " ++ show e ++ " вместо переменной " ++ v
    show (SupposeVariableUsing v e)   = ": используется правило с квантором по переменной " ++ v ++ ", входящей свободно в допущение " ++ show e
    show Unproven = ""


data QRuleMatchREs = QuantRuleSuccess VName Expr Expr | QuantRuleFailed VName Expr Expr| NoQuantRule

showProofError :: (ProofError, Int) -> String
showProofError (e, i) = "Вывод некорректен начиная с формулы " ++ show i ++ show e

annotate :: Proof -> Either (ProofError, Int) AnnProof
annotate pr@(Proof s t p) =  runExcept . evalStateT annotateImpl $ ([], S.empty, 0) where
    annotateImpl :: StateT ([AnnExpr], S.Set Expr, Int) (Except (ProofError, Int)) AnnProof
    annotateImpl = foldr (\e m -> do
            state <- get
            ann <- annotate state e
            addAnnotation e ann
            m
        ) final p

    annotate (annList, exprSet, i) expr
        | True <- L.elem expr s = return Suppose
        | True <- L.elem expr axioma = return Axiom
        | Just _ <- L.find (>< expr) schema = return Axiom
        | Just e <- L.find (\(AnnExpr e' _) -> S.member (e' \-> expr) exprSet) annList = let (AnnExpr e' _ ) = e in return $ ModusPonens e'
        | MatchedAxiom <- qany_schema_check                          = return Axiom
        | FailMatchedAxiom (InvalidSubst e t v) <- qany_schema_check = annotationFailed $ InvalidTermSubtition t v e
        | MatchedAxiom <- qall_schema_check                          = return Axiom
        | FailMatchedAxiom (InvalidSubst e t v) <- qall_schema_check = annotationFailed $ InvalidTermSubtition t v e
        | MatchedAxiom <- induction_check = return Axiom
        | QuantRuleSuccess v l r <- qany_rule_check = return $ ExQuantRule v l r
        | QuantRuleFailed v _ _ <- qany_rule_check = annotationFailed $ FreeVariableForbiden v expr
        | QuantRuleSuccess v l r <- qall_rule_check = return $ UnQuantRule v l r
        | QuantRuleFailed v _ _ <- qall_rule_check = annotationFailed $ FreeVariableForbiden v expr

        | otherwise = annotationFailed Unproven
        where
            qany_schema_check = qany_schema !>< expr
            qall_schema_check = qall_schema !>< expr
            induction_check = induction !>< expr
            quantRule True = QuantRuleSuccess
            quantRule False = QuantRuleFailed
            qany_rule_check = case qany_rule `matchSchema` expr of
                                (Right (xs, exprs)) -> let
                                                        (Just x) = "x" `M.lookup` xs
                                                        (Just a) = "A" `M.lookup` exprs
                                                        (Just b) = "B" `M.lookup` exprs
                                                    in if ((b \-> a) `S.member` exprSet)
                                                        then quantRule (not $ x `S.member`(freeVariables a)) x b a
                                                        else NoQuantRule
                                (Left _) -> NoQuantRule
            qall_rule_check = case qall_rule `matchSchema` expr of
                                (Right (xs, exprs)) -> let
                                                        (Just x) = "x" `M.lookup` xs
                                                        (Just a) = "A" `M.lookup` exprs
                                                        (Just b) = "B" `M.lookup` exprs
                                                    in if ((a \-> b) `S.member` exprSet)
                                                        then quantRule (not $ x `S.member`(freeVariables a)) x a b
                                                        else NoQuantRule
                                (Left _) -> NoQuantRule

    final = do
            (annotations, _, _) <- get
            return $ AnnProof s t (L.reverse annotations)

    addAnnotation e ann = modify (\(anns, set, i) -> ((AnnExpr e ann):anns, S.insert e set, i + 1))
    annotationFailed e = do
                        (_,_,i) <- get
                        throwError (e, i)

deduce :: AnnProof -> Either (ProofError, Int) Proof
deduce pr@(AnnProof s t p) =  runExcept . evalStateT deduceImpl $ ([], L.length p) where
    deduceImpl :: StateT ([Expr], Int) (Except (ProofError, Int)) Proof
    deduceImpl = foldr (\e m -> do {deduceExpr e; m}) final (L.reverse p)

    supposes = L.init s
    a = L.last s
    avars = freeVariables a

    deduceExpr (AnnExpr e Axiom) = addAnswer $ map (\x -> subtituteSchema x $ (M.empty, M.fromAscList [("A", a), ("B", e)])) axDeducing
    deduceExpr (AnnExpr e Suppose) | e == a = addAnswer $ map (\x -> subtituteSchema x $ (M.empty, M.singleton "A" e)) aaDeducing
                                   | otherwise   = addAnswer $ map (\x -> subtituteSchema x $ (M.empty, M.fromAscList [("A", a), ("B", e)])) axDeducing
    deduceExpr (AnnExpr e (ModusPonens sup)) = addAnswer $ map (\x -> subtituteSchema x $ (M.empty, M.fromAscList [("A", a), ("B", sup), ("C", e)])) mpDeducing
    deduceExpr (AnnExpr e (ExQuantRule v l r)) = if (v `S.member` avars)
                                                    then deduceFailed $ SupposeVariableUsing v a
                                                    else addAnswer $ map (\x -> subtituteSchema x $ (M.singleton "x" v, M.fromAscList [("A", a), ("B", l), ("C", r)])) qeDeducing
    deduceExpr (AnnExpr e (UnQuantRule v l r)) =  if (v `S.member` avars)
                                                    then deduceFailed $ SupposeVariableUsing v a
                                                    else addAnswer $ map (\x -> subtituteSchema x $ (M.singleton "x" v, M.fromAscList [("A", a), ("B", l), ("C", r)])) quDeducing

    axDeducing = [ rB
                 , rB /-> rA /-> rB
                 , rA /-> rB
                 ]
    mpDeducing = [ (rA /-> rB) /-> (rA /-> rB /-> rC) /-> rA /-> rC
                 , (rA /-> rB /-> rC) /-> rA /-> rC
                 , rA /-> rC
                 ]
    aaDeducing = [ rA /-> rA /-> rA
                 , (rA /-> rA /-> rA) /-> (rA /-> (rA /-> rA) /-> rA) /-> (rA /-> rA)
                 , (rA /-> (rA /-> rA) /-> rA) /-> (rA /-> rA)
                 , rA /-> (rA /-> rA) /-> rA
                 , rA /-> rA
                 ]
    qex_rB = "x" `quAny` rB
    qun_xC = "x" `quAll` rC
    qeDeducing = [ rB /-> rA /-> rB
                 , rA /-> rB /-> rC
                 , (rA /-> rB /-> rC) /-> rB /-> rA /-> rB /-> rC
                 , rB /-> rA /-> rB /-> rC
                 , (rA /-> rB) /-> (rA /-> rB /-> rC) /-> rA /-> rC
                 , ((rA /-> rB) /-> (rA /-> rB /-> rC) /-> rA /-> rC) /-> rB /-> (rA /-> rB) /-> (rA /-> rB /-> rC) /-> rA /-> rC
                 , rB /-> (rA /-> rB) /-> (rA /-> rB /-> rC) /-> rA/-> rC
                 , (rB /-> rA /-> rB) /-> (rB /-> (rA /-> rB) /-> (rA /-> rB /-> rC) /-> rA /-> rC) /-> rB /-> (rA /-> rB /-> rC) /-> rA /-> rC
                 , (rB /-> (rA /-> rB) /-> (rA /-> rB /-> rC) /-> rA /-> rC) /-> rB /-> (rA /-> rB /-> rC) /-> rA /-> rC
                 , rB /-> (rA /-> rB /-> rC) /-> rA /-> rC
                 , (rB /-> rA /-> rB /-> rC) /-> (rB /-> (rA /-> rB /-> rC) /-> rA /-> rC) /-> rB /-> rA /-> rC
                 , (rB /-> (rA /-> rB /-> rC) /-> rA /-> rC) /-> rB /-> rA /-> rC
                 , rB /-> rA /-> rC
                 , qex_rB /-> rA /-> rC
                 , rA /-> qex_rB /-> rA
                 , qex_rB /-> rA /-> rC
                 , (qex_rB /-> rA /-> rC) /-> rA /-> qex_rB /-> rA /-> rC
                 , rA /-> qex_rB /-> rA /-> rC
                 , (qex_rB /-> rA) /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC
                 , ((qex_rB /-> rA) /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC) /-> rA /-> (qex_rB /-> rA) /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC
                 , rA /-> (qex_rB /-> rA) /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC
                 , (rA /-> qex_rB /-> rA) /-> (rA /-> (qex_rB /-> rA) /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC) /-> rA /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC
                 , (rA /-> (qex_rB /-> rA) /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC) /-> rA /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC
                 , rA /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC
                 , (rA /-> qex_rB /-> rA /-> rC) /-> (rA /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC) /-> rA /-> qex_rB /-> rC
                 , (rA /-> (qex_rB /-> rA /-> rC) /-> qex_rB /-> rC) /-> rA /-> qex_rB /-> rC
                 , rA /-> qex_rB /-> rC
                 ]
    quDeducing = [ rA /-> rB /-> rC
                 , rA /& rB /-> rA
                 , rA /& rB /-> rB
                 , rA /-> rB /-> rC
                 , (rA /-> rB /-> rC) /-> rA /& rB /-> rA /-> rB /-> rC
                 , rA /& rB /-> rA /-> rB /-> rC
                 , (rA /& rB /-> rA) /-> (rA /& rB /-> rA /-> rB /-> rC) /-> rA /& rB /-> rB /-> rC
                 , (rA /& rB /-> rA /-> rB /-> rC) /-> rA /& rB /-> rB /-> rC
                 , rA /& rB /-> rB /-> rC
                 , (rA /& rB /-> rB) /-> (rA /& rB /-> rB /-> rC) /-> (rA /& rB /-> rC)
                 , (rA /& rB /-> rB /-> rC) /-> (rA /& rB /-> rC)
                 , rA /& rB /-> rC
                 , rA /& rB /-> qun_xC
                 , rA /-> rB /-> rA /& rB
                 , rA /& rB /-> qun_xC
                 , (rA /& rB /-> qun_xC) /-> rA /-> rA /& rB /-> qun_xC
                 , rA /-> rA /& rB /-> qun_xC
                 , (rA /& rB /-> qun_xC) /-> rB /-> rA /& rB /-> qun_xC
                 , ((rA /& rB /-> qun_xC) /-> rB /-> rA /& rB /-> qun_xC) /-> rA /-> (rA /& rB /-> qun_xC) /-> rB /-> rA /& rB /-> qun_xC
                 , rA /-> (rA /& rB /-> qun_xC) /-> rB /-> rA /& rB /-> qun_xC
                 , (rA /-> rA /& rB /-> qun_xC) /-> (rA /-> ((rA /& rB /-> qun_xC) /-> rB /-> rA /& rB /-> qun_xC)) /-> rA /-> rB /-> rA /& rB /-> qun_xC
                 , (rA /-> ((rA /& rB /-> qun_xC) /-> rB /-> rA /& rB /-> qun_xC)) /-> rA /-> rB /-> rA /& rB /-> qun_xC
                 , rA /-> rB /-> rA /& rB /-> qun_xC
                 , (rB /-> rA /& rB) /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC
                 , ((rB /-> rA /& rB) /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC) /-> rA /-> (rB /-> rA /& rB) /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC
                 , rA /-> (rB /-> rA /& rB) /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC
                 , (rA /-> rB /-> rA /& rB) /-> (rA /-> (rB /-> rA /& rB) /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC) /-> rA /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC
                 , (rA /-> (rB /-> rA /& rB) /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC) /-> rA /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC
                 , rA /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC
                 , (rA /-> rB /-> rA /& rB /-> qun_xC) /-> (rA /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC) /-> rA /-> rB /-> qun_xC
                 , (rA /-> (rB /-> rA /& rB /-> qun_xC) /-> rB /-> qun_xC) /-> rA /-> rB /-> qun_xC
                 , rA /-> rB /-> qun_xC
                 ]

    addAnswer e = modify (\(e', i') -> (e ++ e', i' - 1))
    final = do
                (res, _) <- get
                return $ Proof supposes (a \-> t) res
    deduceFailed e =  do
                    (_, i) <- get
                    throwError (e, i)

parseProof :: String -> Either ParseError Proof
parseProof = parse proofParser ""

proofParser :: Parsec String () Proof
proofParser = do
                supposses <- implication `sepBy` (string ",")
                string "|-"
                expr <- implication
                string "\n"
                proof <- implication `sepBy` (string "\n")
                return (Proof supposses expr proof)