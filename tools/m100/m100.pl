:- include('listlib.pl').

listToNumberAll([First | Tail], Number) :-
	((
	First >= 0,
	listToNumber([First | Tail], Number)
	);(
	First < 0,
	FirstNew is -First,
	listToNumber([FirstNew | Tail], Num),
	Number is -Num
	)).

/**************************
 * local n2l - first num +-
 **************************/
n2l(Number, List) :-
	numberToList(Number, L),
	L = [H | Tail],
	(
	(H = 0, List = L)
	;
	(H =\= 0, 
		(List = L; 
		HNew is -H, List = [HNew | Tail]
		)
	)
	).

operator(Operator) :-
	member(Operator, [+, -, *, /]).

formula(List, X) :-
	formula(List, [], X).
formula([], [A], A).
formula([H | Tail], A, X) :-
	formula(Tail, [H | A], X).
formula(Tail, [An, Am | A], X) :-
	operator(Operator),
	Form =.. [Operator, Am, An],
	formula(Tail, [Form | A], X).

makeAnsList(NumList, Result, Formula) :-
	shatter(NumList, SubList),
	maplist(listToNumberAll, SubList, List),
	formula(List, Formula),
	catch(Result_t is Formula, _, fail),
	Result =:= Result_t,
	!.

makeAns(Num, Result, Formula) :-
	n2l(Num, NumList),
	makeAnsList(NumList, Result, Formula).

write_num(Num, OS) :-
	forall(member(X, Num),
		write(OS, X)),
	nl(OS),
	writeln(Num).

main(N) :-
	open('m100.log', write, OS),
	(
	length(Nums, N),
	get_set(NumList, Nums,
			[0,1,2,3,4,5,6,7,8,9]),
	listToNumber(NumList, Num),
	\+makeAns(Num, 100, _),
	write_num(NumList, OS),
	fail
	;
	close(OS)
	).

% 	usage
% allFormulas([], L, X, R), format('~w~t~30| = ~w~n', [X, R]), fail.
