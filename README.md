<h1>HashChecker</h1>
<h5>RUS</h5>
<p>HashChecker проверяет целостность файлов сраванивая хэш-суммы из файла, который подается на вход, с хэш-суммами вычисленными по соответствующему алгоритму (MD5/SHA1/SHA256)</p> 

Пример файла, который подается на вход:
```
dsa.txt     sha1   194a8b56549222d93361bd9f34174fa1928fa0b5

funny.png   sha1   9a24f0523d1a903b631c3f57c7f63b9817e0b287

db1.accdb   md5    0f235eb5fef9b79fea29d3716108386fb66994da

IEShims.dll sha256 ac1abd33454cbcee7cdbd60bdd7681094e9195ec67286349d51756302d4ac448
```
Пример вызова:
```
HashChecker <путь к input файлу> <путь к директории с файлами для проверки>
```
Пример вывода:
```
dsa.txt     OK
funny.png   FAIL
db1.accdb   NOT FOUND
IEShims.dll OK
```

<h5>ENG</h5>
<p>HashChecker checks files if they corrupted or not, by comparison, two hashes(MD5/SHA1/SHA256): one provided by input file and the one, calculated by the programm.</p> 

Example of an input file:
```
dsa.txt     sha1   194a8b56549222d93361bd9f34174fa1928fa0b5

funny.png   sha1   9a24f0523d1a903b631c3f57c7f63b9817e0b287

db1.accdb   md5    0f235eb5fef9b79fea29d3716108386fb66994da

IEShims.dll sha256 ac1abd33454cbcee7cdbd60bdd7681094e9195ec67286349d51756302d4ac448
```
Example of execution:
```
HashChecker <path to an input file> <path to the directory containing files to check>
```
Example of an output:
```
dsa.txt     OK
funny.png   FAIL
db1.accdb   NOT FOUND
IEShims.dll OK
```
