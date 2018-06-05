# Server
Korzysta z socketów, pthreads i C++14.  
Platforma decelowa: Linux.

Serwer nasłuchuje na połączenia ze strony klientów na porcie 5050 (TCP).

### Submodules
Projekt zawiera w sobie inne repozytoria i zwykły clone ich nie pobiera.
Aby ułatwić ich ściąganie został stworzony skrypt configure.sh.

Uruchomienie skryptu
```
./configure.sh
```

### Kompilacja
```
mkdir build
cd build
cmake ..
make
```

### Testowanie
```
ctest
```

### Uruchomienie
```
./Locus
```

### Wymagania
* Kompilator wspierający standard **C++14**  
* Narzędzie **CMake**
