module Main where

import System.Environment
import Proof

main = do
        [proofFile] <- getArgs
        proofText <- readFile proofFile
        let proof' = parseProof proofText
        case proof' of
            (Left e) -> putStrLn . show $ e
            (Right proof@(Proof s e p)) -> do
                let annotProof' = annotate proof
                case annotProof' of
                    (Left e) -> putStrLn . showProofError $ e
                    (Right annotProof) -> if length s == 0
                        then putStr . show $ proof
                        else case deduce annotProof of
                            (Left e) -> putStrLn . showProofError $ e
                            (Right deduced) -> putStr . show $ deduced