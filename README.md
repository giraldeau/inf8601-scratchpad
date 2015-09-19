# INF8601 Scratchpad

Demo and examples for the INF8601 course at Polytechnique Montréal.

## Compile the sources

First, install the dependencies. On Ubuntu Linux:

```
apt-get install build-essential qtcreator qtbase5-dev libtbb-dev
```

Open the project file inf8601-scratchpad.pro with qtcreator. The build is a
shadow build by default, meaning that the binaries are located outside the
source directory.

## Add example

Examples are defined as a subproject. They are numbered to keep them sorted.

## Tracing and performance measurement

* Instructions to install and configure LTTng, refer to http://lttng.org/download/
* Linux Perf (performance counters, either software or hardware)
```
sudo apt-get install linux-tools-$(uname -r)
```

* Valgrind and KCacheGrind
```
sudo apt-get install valgrind kcachegrind
```
