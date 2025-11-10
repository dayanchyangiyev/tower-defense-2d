# Tower Defense 2D


### Descriere

Bun venit la 2D Tower Defense! Acesta este un joc clasic de strategie în care obiectivul tău principal este să îți protejezi baza de inamicii invadatori. Te vei confrunta cu o armată necruțătoare, iar inteligența și planificarea ta sunt tot ce stă între fortăreața ta și distrugerea totală.

Jocul se desfășoară pe o hartă 2D care prezintă o cale prestabilită. Hoarde de inamici vor apărea în valuri și vor urmări automat această cale, încercând să ajungă la baza ta aflată la celălalt capăt. Pentru a-i opri, trebuie să plasezi strategic o varietate de turnuri automate pe zonele desemnate pentru construcție de-a lungul căii. Aceste turnuri vor detecta și ataca automat orice inamic care intră în raza lor de acțiune.

Strategia este cheia. Fiecare inamic pe care îl învingi te recompensează cu bani, care reprezintă resursa principală a jocului. Trebuie să folosești acești bani cu înțelepciune pentru a construi turnuri noi sau pentru a le îmbunătăți pe cele existente. Diferite turnuri au puncte forte unice - unele atacă rapid, altele provoacă daune masive unei singure ținte, iar altele pot încetini inamicii. A decide ce să construiești și unde să construiești este provocarea principală. O plasare bună poate crea "zone de foc" unde mai multe turnuri pot ataca aceiași inamici, maximizând daunele.

Obiectivul tău este să supraviețuiești fiecărui val de atacatori. Pe măsură ce progresezi, valurile vor deveni mai mari și vor include tipuri de inamici mai puternici, mai rapizi și mai rezistenți. Va trebui să îți adaptezi strategia, să investești în îmbunătățiri puternice și să îți gestionezi economia pentru a contracara amenințarea tot mai mare. Dacă poți rezista asaltului final și îți poți menține baza în picioare, câștigi jocul!

### Tema 0

- [ ] Nume proiect (poate fi schimbat ulterior)
- [ ] Scurtă descriere a temei alese, ce v-ați propus să implementați

## Tema 1

#### Cerințe
- [ ] definirea a minim **3-4 clase** folosind compunere cu clasele definite de voi; moștenirile nu se iau în considerare aici
- [ ] constructori de inițializare cu parametri pentru fiecare clasă
- [ ] pentru o aceeași (singură) clasă: constructor de copiere, `operator=` de copiere, destructor
<!-- - [ ] pentru o altă clasă: constructor de mutare, `operator=` de mutare, destructor -->
<!-- - [ ] pentru o altă clasă: toate cele 5 funcții membru speciale -->
- [ ] `operator<<` pentru **toate** clasele pentru afișare (`std::ostream`) folosind compunere de apeluri cu `operator<<`
- [ ] cât mai multe `const` (unde este cazul) și funcții `private`
- [ ] implementarea a minim 3 funcții membru publice pentru funcționalități netriviale specifice temei alese, dintre care cel puțin 1-2 funcții mai complexe
  - nu doar citiri/afișări sau adăugat/șters elemente într-un/dintr-un vector
- [ ] scenariu de utilizare **cu sens** a claselor definite:
  - crearea de obiecte și apelarea tuturor funcțiilor membru publice în main
  - vor fi adăugate în fișierul `tastatura.txt` DOAR exemple de date de intrare de la tastatură (dacă există); dacă aveți nevoie de date din fișiere, creați alte fișiere separat
- [ ] minim 50-55% din codul propriu să fie C++, `.gitattributes` configurat corect
- [ ] tag de `git`: de exemplu `v0.1`
- [ ] serviciu de integrare continuă (CI) cu **toate bifele**; exemplu: GitHub Actions
- [ ] code review #1 2 proiecte

## Tema 2

#### Cerințe
- [ ] separarea codului din clase în `.h` (sau `.hpp`) și `.cpp`
- [ ] moșteniri:
  - minim o clasă de bază și **3 clase derivate** din aceeași ierarhie
  - ierarhia trebuie să fie cu bază proprie, nu derivată dintr-o clasă predefinită
  - [ ] funcții virtuale (pure) apelate prin pointeri de bază din clasa care conține atributul de tip pointer de bază
    - minim o funcție virtuală va fi **specifică temei** (i.e. nu simple citiri/afișări sau preluate din biblioteci i.e. draw/update/render)
    - constructori virtuali (clone): sunt necesari, dar nu se consideră funcții specifice temei
    - afișare virtuală, interfață non-virtuală
  - [ ] apelarea constructorului din clasa de bază din constructori din derivate
  - [ ] clasă cu atribut de tip pointer la o clasă de bază cu derivate; aici apelați funcțiile virtuale prin pointer de bază, eventual prin interfața non-virtuală din bază
    - [ ] suprascris cc/op= pentru copieri/atribuiri corecte, copy and swap
    - [ ] `dynamic_cast`/`std::dynamic_pointer_cast` pentru downcast cu sens
    - [ ] smart pointers (recomandat, opțional)
- [ ] excepții
  - [ ] ierarhie proprie cu baza `std::exception` sau derivată din `std::exception`; minim **3** clase pentru erori specifice distincte
    - clasele de excepții trebuie să trateze categorii de erori distincte (exemplu de erori echivalente: citire fișiere cu diverse extensii)
  - [ ] utilizare cu sens: de exemplu, `throw` în constructor (sau funcție care întoarce un obiect), `try`/`catch` în `main`
  - această ierarhie va fi complet independentă de ierarhia cu funcții virtuale
- [ ] funcții și atribute `static`
- [ ] STL
- [ ] cât mai multe `const`
- [ ] funcții *de nivel înalt*, de eliminat cât mai mulți getters/setters/funcții low-level
- [ ] minim 75-80% din codul propriu să fie C++
- [ ] la sfârșit: commit separat cu adăugarea unei noi clase derivate fără a modifica restul codului, **pe lângă cele 3 derivate deja adăugate** din aceeași ierarhie
  - noua derivată nu poate fi una existentă care a fost ștearsă și adăugată din nou
  - noua derivată va fi integrată în codul existent (adică va fi folosită, nu adăugată doar ca să fie)
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.2`
- [ ] code review #2 2 proiecte

## Tema 3

#### Cerințe
- [ ] 2 șabloane de proiectare (design patterns)
- [ ] o clasă șablon cu sens; minim **2 instanțieri**
  - [ ] preferabil și o funcție șablon (template) cu sens; minim 2 instanțieri
- [ ] minim 85% din codul propriu să fie C++
<!-- - [ ] o specializare pe funcție/clasă șablon -->
- [ ] tag de `git` pe commit cu **toate bifele**: de exemplu `v0.3` sau `v1.0`
- [ ] code review #3 2 proiecte

## Instrucțiuni de compilare

Proiectul este configurat cu CMake.

- **macOS**: rulați `./scripts/cmake.sh configure -c Debug`, apoi `./scripts/cmake.sh build -c Debug`. SDL3 și SDL3_image sunt livrate în `lib/SDL3`, iar CMake le va lega automat ca framework-uri.
- **Windows / Ubuntu**: urmați pașii standard CMake (configurați prin `cmake -S . -B build` și compilați cu generatorul preferat) și instalați dependențele SDL3 din mediul vostru.

## Instrucțiuni pentru a rula executabilul

Executabilul citește datele de intrare din `tastatura.txt`, astfel încât puteți reproduce rularea din CI cu `cat tastatura.txt | ./build/simple_game` (sau `.exe` pe Windows).

## Resurse

