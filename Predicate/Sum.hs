module Main where

import System.Environment

import qualified Data.Text as T

abba =
    [ "0=0->0=0->0=0"
    , "a=b->a=c->b=c"
    , "(a=b->a=c->b=c)->(0=0->0=0->0=0)->(a=b->a=c->b=c)"
    , "(0=0->0=0->0=0)->(a=b->a=c->b=c)"
    , "(0=0->0=0->0=0)->@c(a=b->a=c->b=c)"
    , "(0=0->0=0->0=0)->@b@c(a=b->a=c->b=c)"
    , "(0=0->0=0->0=0)->@a@b@c(a=b->a=c->b=c)"
    , "@c(a=b->a=c->b=c)"
    , "@c(a=b->a=c->b=c)->(a=b->a=a->b=a)"
    , "a=b->a=a->b=a"
    , "@a@b@c(a=b->a=c->b=c)"
    , "@a@b@c(a=b->a=c->b=c)->@b@c(a+0=b->a+0=c->b=c)"
    , "(0=0->0=0->0=0)->@a@b@c(a=b->a=c->b=c)"
    , "@b@c(a+0=b->a+0=c->b=c)->@c(a+0=a->a+0=c->a=c)"
    , "@b@c(a+0=b->a+0=c->b=c)"
    , "@c(a+0=a->a+0=c->a=c)"
    , "@c(a+0=a->a+0=c->a=c)->(a+0=a->a+0=a->a=a)"
    , "a+0=a->a+0=a->a=a"
    , "a+0=a"
    , "a+0=a->a=a"
    , "a=a"
    , "a=a->a=b->a=a"
    , "a=b->a=a"
    , "(a=b->a=a)->(a=b->a=a->b=a)->(a=b->b=a)"
    , "(a=b->a=a->b=a)->(a=b->b=a)"
    , "a=b->b=a"
    , "(a=b->b=a)->(0=0->0=0->0=0)->(a=b->b=a)"
    , "(0=0->0=0->0=0)->(a=b->b=a)"
    , "(0=0->0=0->0=0)->@b(a=b->b=a)"
    , "(0=0->0=0->0=0)->@a@b(a=b->b=a)"
    , "@a@b(a=b->b=a)"
    , "@a@b(a=b->b=a)->@b(x=b->b=x)"
    , "@b(x=b->b=x)"
    , "@b(x=b->b=x)->(x=y->y=x)"
    , "x=y->y=x"
    , "(x=y->y=x)->(0=0->0=0->0=0)->(x=y->y=x)"
    , "(0=0->0=0->0=0)->(x=y->y=x)"
    , "(0=0->0=0->0=0)->@y(x=y->y=x)"
    , "(0=0->0=0->0=0)->@x@y(x=y->y=x)"
    , "@x@y(x=y->y=x)"
    ]

stepStr =
    [ "a+b'=(a+b)'"
    , "a+b'=(a+b)'->(A->B->A)->a+b'=(a+b)'"
    , "(A->B->A)->a+b'=(a+b)'"
    , "(A->B->A)->@b(a+b'=(a+b)')"
    , "A->B->A"
    , "@b(a+b'=(a+b)')"
    , "@b(a+b'=(a+b)')->(a+o'=(a+o)')"
    , "a+o'=(a+o)'"
    , "@x@y(x=y->y=x)->@y((a+o')=y->y=(a+o'))"
    , "@y((a+o')=y->y=(a+o'))"
    , "@y((a+o')=y->y=(a+o'))->(a+o')=(a+o)'->(a+o)'=(a+o')"
    , "(a+o')=(a+o)'->(a+o)'=(a+o')"
    , "(a+o)'=(a+o')"
    , "a=b->a=c->b=c"
    , "(a=b->a=c->b=c)->(A->B->A)->(a=b->a=c->b=c)"
    , "(A->B->A)->(a=b->a=c->b=c)"
    , "(A->B->A)->@c(a=b->a=c->b=c)"
    , "(A->B->A)->@b@c(a=b->a=c->b=c)"
    , "(A->B->A)->@a@b@c(a=b->a=c->b=c)"
    , "(A->B->A)"
    , "@a@b@c(a=b->a=c->b=c)"
    , "@a@b@c(a=b->a=c->b=c)->@b@c((a+o)'=b->(a+o)'=c->b=c)"
    , "@b@c((a+o)'=b->(a+o)'=c->b=c)"
    , "@b@c((a+o)'=b->(a+o)'=c->b=c)->@c((a+o)'=(a+o')->(a+o)'=c->(a+o')=c)"
    , "@c((a+o)'=(a+o')->(a+o)'=c->(a+o')=c)"
    , "@c((a+o)'=(a+o')->(a+o)'=c->(a+o')=c)->((a+o)'=(a+o')->(a+o)'=d'->(a+o')=d')"
    , "((a+o)'=(a+o')->(a+o)'=d'->(a+o')=d')"
    , "(a+o)'=d'->(a+o')=d'"
    , "a+o=d"
    , "a=b->a'=b'"
    , "(a=b->a'=b')->(A->B->A)->(a=b->a'=b')"
    , "(A->B->A)->(a=b->a'=b')"
    , "(A->B->A)->@b(a=b->a'=b')"
    , "(A->B->A)->@a@b(a=b->a'=b')"
    , "@a@b((a=b)->(a'=b'))"
    , "@a@b((a=b)->(a'=b'))->@b((a+o=b)->(a+o)'=b')"
    , "@b((a+o=b)->(a+o)'=b')"
    , "@b((a+o=b)->(a+o)'=b')->((a+o=d)->(a+o)'=d')"
    , "(a+o=d)->(a+o)'=d'"
    , "(a+o)'=d'"
    , "a+o'=d'"
    ]


finalStr =
    [ "a+o=d"
    ,"(a+o=d)->(A->B->A)->(a+o=d)"
    , "(A->B->A)->(a+o=d)"
    , "(A->B->A)->@a(a+o=d)"
    , "@a(a+o=d)"
    , "@a(a+o=d)->(q+o=f)"
    , "q+o=f"
    ]


succN t n = t `T.append` T.pack (replicate n '\'')

putStrList = foldl (\m s -> m >> putStrLn s) (return ())


o = T.singleton 'o'
zero = T.singleton '0'
d = T.singleton 'd'
a = T.singleton 'a'
q = T.singleton 'q'
f = T.singleton 'f'

step i = map (T.unpack . (T.replace o $ succN zero i) . (T.replace d $ succN a i) . T.pack) stepStr

main = do
        [aStr, bStr] <- getArgs
        let a' = read aStr  :: Int
        let b' = read bStr  :: Int
        putStrList abba
        foldl (\m i -> do {m; (putStrList $ step i)}) (return ()) [0.. b' - 1]
        let ending = map ( T.unpack
                         . (T.replace o $ succN zero b')
                         . (T.replace d $ succN a b')
                         . (T.replace q $ succN zero a')
                         . (T.replace f $ succN zero (a' + b'))
                         . T.pack
                         ) finalStr
        putStrList ending





