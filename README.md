# Aerospike C Client API

This repository is a collection of C client libraries for interfacing with Aerospike database clusters.

## BTJ Notes:
Build currently fails. Last at:
```
(cpython27) bjolit@BJOLIT1013mac:[aerospike-client-c] (osx-port)$ make CFLAGS+='-DOSX -I/usr/local/include' LDFLAGS+='-L/usr/local/lib -lcrypto -llua -lz' -C citrusleaf-base all
ar rcs target/Darwin-i386/lib/libaerospike-base.a target/Darwin-i386/obj/base/cf_alloc.o target/Darwin-i386/obj/base/cf_average.o target/Darwin-i386/obj/base/cf_client_rc.o target/Darwin-i386/obj/base/cf_hist.o target/Darwin-i386/obj/base/cf_log.o target/Darwin-i386/obj/base/cf_proto.o target/Darwin-i386/obj/base/cf_queue.o target/Darwin-i386/obj/base/cf_random.o target/Darwin-i386/obj/base/cf_service.o target/Darwin-i386/obj/base/cf_socket.o target/Darwin-i386/obj/base/cf_packet_compression.o
gcc -shared -Isrc/include -I/Users/bjolit/aerospike-client-c/aerospike-common/target/Darwin-i386/include -Wall -Winline -rdynamic -L/usr/local/lib -lcrypto -llua -lz -o target/Darwin-i386/lib/libaerospike-base.so target/Darwin-i386/obj/base/cf_alloc.o target/Darwin-i386/obj/base/cf_average.o target/Darwin-i386/obj/base/cf_client_rc.o target/Darwin-i386/obj/base/cf_hist.o target/Darwin-i386/obj/base/cf_log.o target/Darwin-i386/obj/base/cf_proto.o target/Darwin-i386/obj/base/cf_queue.o target/Darwin-i386/obj/base/cf_random.o target/Darwin-i386/obj/base/cf_service.o target/Darwin-i386/obj/base/cf_socket.o target/Darwin-i386/obj/base/cf_packet_compression.o
Undefined symbols for architecture x86_64:
  "_cf_bits_find_last_set_64", referenced from:
      _cf_histogram_insert_data_point in cf_hist.o
  "_cf_getms", referenced from:
      _cf_histogram_insert_data_point in cf_hist.o
      _cf_queue_pop in cf_queue.o
      _cf_socket_read_timeout in cf_socket.o
      _cf_socket_write_timeout in cf_socket.o
ld: symbol(s) not found for architecture x86_64
clang: error: linker command failed with exit code 1 (use -v to see invocation)
make: *** [target/Darwin-i386/lib/libaerospike-base.so] Error 1
(cpython27) bjolit@BJOLIT1013mac:[aerospike-client-c] (osx-port)$ 
```




## Modules

The C Client repository is composed of multiple modules. Each module is either a client library or shared module.

- **[aerospike](./aerospike)** – Aerospike (3.x) C API 
- **[citrusleaf-base](./citrusleaf-base)** – Shared files for citrusleaf submodules.
- **[citrusleaf-client](./citrusleaf-client)** – Citrusleaf (2.x) C API (*deprecated*)
- **[citrusleaf-libevent](./citrusleaf-client)** – libevent-based API

Each module has its own `README.md` and `Makefile`. 

Please read the `README.md` for each module before using them or running make. The document contains information on prerequisites, usage and directory structure.

## Usage

Please ensure you have resolved the prerequisites and dependencies in the README.me for each module before running commands accross all modules.

### Cloning

To clone this repository, run:

	$ git clone --recursive https://github.com/aerospike/aerospike-client-c.git

### Build

To build all modules:

	$ make

To build on OSX (this repo):
	   $ brew install lua
       $ make CFLAGS='-DOSX -I/usr/local/include' LDFLAGS='-L/usr/local/lib -lcrypto -llua -lz'

To build a specific module:

	$ make -C {module}

### Clean

To clean all modules:

	$ make clean

To clean a specific module:

	$ make -C {module} clean

### Other Targets

To run `{target}` on all module:

	$ make {target}

To run `{target}` on a specific module:

	$ make -C {module} {target}

