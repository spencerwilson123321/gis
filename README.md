# gis
A Geographical Information System (GIS) implemented in C++ as part of COMP 8047 at BCIT. A geographical information system processes geographical data so that it can be efficiently queried and utilized. This particular application uses a custom PR Bucket QuadTree implementation as well as a custom hash table implementation using quadratic probing to index the geographical data. Once the data is indexed, the user uses a "command file" to issue queries to the indexed data and the application quickly and efficiently returns the results.

# Compile (Linux)
The software was developed for and only tested on Linux. To compile the software use the build script:

```
chmod +x build.sh;
./build.sh;
```

# Usage
```
./GIS <database file> <command file> <log file>
```