echo Building $BUILD package
md crossuo-win64-$BUILD
copy release/src/crossuo.so crossuo-win64-$BUILD/
copy release/src/crossuo crossuo-win64-$BUILD/
copy release/tools/xuoi/xuolauncher crossuo-win64-$BUILD/
7z a crossuo-win64-%BUILD%.zip crossuo-win64-$BUILD/