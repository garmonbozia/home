:- include(  '../../libs/listlib.pl'  ).

digits( [0,1,2,3,4,5,6,7,8,9] ).

listToNumberAll( [First | Tail], Number ) :-
	( First >= 0
    , listToNumber( [First | Tail], Number )
    )
    ;
    ( First < 0
    , FirstNew is -First
    , listToNumber( [FirstNew | Tail], Num )
    , Number is -Num
	).

/**************************
 * local n2l - first num +-
 **************************/
n2l( Number, List ) :-
	numberToList( Number, L ),
	L = [H | Tail],
	( ( H = 0
      , List = L
      )
	  ;
	  ( H =\= 0
      , ( List = L
        ; HNew is -H
        , List = [HNew | Tail]
		)
	)
	).

expression( List, X ) :-
    expressionH(List, [], X).

expressionH( [], [A], A ).
expressionH( [H | Tail], A, X ) :-
    expressionH( Tail, [H | A], X ).
expressionH( Head, [An, Am | A], X ) :-
    member( Operator, [+, -, *, /] ),
    Form =.. [Operator, Am, An],
    expressionH( Head, [Form | A], X ).

/*
expression( List, Expression ) :-
    reverse( List, ListRev),
    expressionH( ListRev, Expression).
expressionH( [X1, X2 | Tail], Acc ) :-
     member( Operator, [+, -, *, /] ),
     F =.. [Operator, X2, X1],
     expressionH( [F | Tail], Acc ).
expressionH( [Expr], Expr ).
*/

%% makeAnsList( +NumList, +Result, -Formula )
%
% generate Formula (got from Numlist), that equal Result
makeAnsList( NumList, Result, Formula ) :-
	shatter( NumList, SubList ),
	maplist( listToNumberAll, SubList, List ),
	expression( List, Formula ),
	% ignoring evaluation error: `zero_divisor'
    catch( Result is Formula, _, false ).

unsolvebleNumList( NumList, Result ) :-
	\+makeAnsList( NumList, Result, _ ).

solvebleNumList( NumList, Result ) :-
    makeAnsList( NumList, Result, _ ),
    !.

solveNum( Num, Result, Formula ) :-
	n2l( Num, NumList ),
	makeAnsList( NumList, Result, Formula ).

solvebleNum( Num, Result ) :-
    solveNum( Num, Result, _ ).

unsolvebleNum( Num, Result ) :-
    \+solveNum( Num, Result, _ ).

writeNumList( NumList, OS ) :-
	forall( member( X, NumList ), write( OS, X ) ),
	nl( OS ).

m100( Num, Formula ) :-
    solveNum( Num, 100, Formula).

unsolvebleFixedLength( N, NumList ) :-
    length( Nums, N ),
    digits( Digits ),
    get_set( NumList, Nums, Digits ),
    listToNumber( NumList, Number ),
    num_length( Number, N ),
    unsolvebleNumList( NumList, 100 ).

%% formulasCount( +NumList, -Count )
%
% counting quantity of all formulas 
% possible to create from NumList

formulasCount( N, Count ) :-
    length( List, N ),
    aggregate( count
             , F^( shatter( List, SubList )
                 , maplist( listToNumberAll, SubList, L )
                 , expression( L, F )
                 )
            , Count
             ).

allFixedLength( N ) :-
    open( 'm100_all.log', write, OS ),
    (
        length( Nums, N ),
        digits( Digits ),
        get_set( NumList, Nums, Digits ),
        listToNumber( NumList, Number ),
        num_length( Number, N ),
        format( OS, 'num: ~w, solved: ', [Number] ),
        %write( OS, 'num: '),
        %write( OS, Number ),
        %write( OS, ', solved: ' ),
        (
        (
            unsolvebleNumList( NumList, 100 ),
            write( OS, '-' )
        )
            ;
        (
            \+unsolvebleNumList( NumList, 100 ),
            write( OS, '+' )
        )
        ),
        nl( OS ),
        fail
    ;
        close( OS )
    ).

allUnsolvebleFixedLength( N ) :-
    open( 'm100.unsolveble.log', write, OS ),
    (
        unsolvebleFixedLength( N, NumList ),
        writeNumList( NumList, OS ),
        writeln( NumList ),
        fail
    ;
        close( OS )
    ).

main( N ) :-
	open( 'm100.log', write, OS ),
	( 
	length( Nums, N ),
    digits( Digits ),
	get_set( NumList, Nums, Digits ),
    unsolvebleNumList( NumList, 100 ),
	writeNumList( NumList, OS ),
	writeln( NumList ),
	fail
	;
	close( OS )
	).

