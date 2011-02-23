prefix(X, L) :- append(X, _, L).
suffix(X, L) :-	append(_, X, L).

sublistCon(X, L) :- 
	suffix(S, L),
	prefix(X, S).

sublist([], []).
sublist([H | Tail1], [H | Tail2]) :-
	sublist(Tail1, Tail2).
sublist(H, [_ | Tail]) :-
	sublist(H, Tail).

get_set(X, L) :- 
	get_set(X, L, L).
get_set([], [], _).
get_set([X | Xs], [_|T], L) :-
	member(X, L),
	get_set(Xs, T, L).	

shatter(InList, OutLists) :-
	length(InList, InL),
	between(1, InL, OutL),
	length(OutLists, OutL),
	append(OutLists, InList),
	forall(
		member(SubOutLists, OutLists),
		 SubOutLists \= []
	).

num_length(X, N) :- N is floor(log10(X) + 1).

listToNumber(List, Number) :- 
	listToNumber(List, 0, Number).
listToNumber([], X, X).
listToNumber([H | Tail], A, X) :-
	Anew is A * 10 + H,
	listToNumber(Tail, Anew, X).

numberToList(X, List) :-
	num_length(X, N),
	numberToList(X, List, N).
numberToList(Number, List, Length) :-
	numberToList(Number, [], List, Length).
numberToList(_, X, X, 0).
numberToList(Number, A, X, Length) :-
	NumberNew is Number // 10,
	R is Number rem 10,
	append([R], A, Anew),
	Length =\= 0,
	LengthNew is Length - 1,
	numberToList(NumberNew, Anew, X, LengthNew).
