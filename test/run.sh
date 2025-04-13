rm -f CppHttpFileServer
cp ../build/CppHttpFileServer  .
lsof -i:8080
./CppHttpFileServer
