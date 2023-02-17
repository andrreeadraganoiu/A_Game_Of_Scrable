Draganoiu Andreea 312CD
A Game of Scrabble

link : https://ocw.cs.pub.ro/courses/programare/teme_2020/tema2_2020_cbd

In functie de numarul taskului am citit pe a doua si a treia linie 
substringurile(daca task este intre 3 si 6). Daca este  intre 1 si 5 
citesc mai departe coordonatele, directia si cuvantul, iar in aceeasi 
functie (read_one_word()) aflu si daca cuvantul se incadreaza la unul 
dintre cele 2 bonusuri, aflu pozitia lui in vectorul caracteristic 
word_used[](daca se afla) corespunzatoare vectorului predefinit de 
cuvinte words (nu le folosesc la primele 
task-uri). Apoi plasez cuvintele din input pe tabla de joc.

Task0 - Am initializat tabla mai sus si imi ramane sa o afisez.

Task1 - Am citit cuvintele si le-am plasat pe tabla de joc mai sus.

Task2 - Am calculat scorul jucatorilor fara bonus.

Task3 - Am calculat scorul cu bonus ( bonus1 se refera la primul substring
care se poate afla oriunde in componenta cuvantului, iar bonus2 la cel
de-al doilea care se poate afla doar la sfarsitul cuvantului).

Task4 - Caut primul cuvant din vectorul predefinit words care nu se da la
input.(am populat vectorul caracteristic word_used[] cu 1 atunci cand am
citit cate un cuvant in functia read_one_word())

Task5 - Caut cuvantul cu valoarea cea mai mare din words. Daca acest cuvant nu 
aduce remiza sau castigul pentru Player2 afisez Fail!. Find_next_optimal_word()
din functia task5() imi parcurge vectorul de cuvinte, alege un cuvant si cu
ajutorul functiei check_optimal_word() determina valoarea maxima a cuvantului
respectiv determinata de asezarea lui pe tabla de joc. In functia get_max_word()
folosita in check_optimal_word() am apelat place_one_word() pentru ca acesta
functie imi completeaza si vectorii coordonatelor fiecarei litere din cuvant 
x[] si y[] pentru ca pentru fiecare cuvant vreau sa determin pozitia 
care imi genereaza valoarea maxima. Ca sa sterg apoi cuvantul de pe tabla
folosesc clear_word(). Practic am 2 maxime, maximul unui cuvant si 
maximul tuturor cuvintelor. 

Task6 - Citesc un cuvant pentru Player1 si caut un cuvant optim pentru 
Player2(conditiile de la task5). Pentru fiecare player calculez valoarea
fiecarui cuvant iar apoi scorul.
 