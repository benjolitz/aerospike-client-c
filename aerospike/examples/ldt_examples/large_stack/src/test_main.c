/*  Citrusleaf General Performance Test Program
 *
 *  Tailored for the  Large Object Stack Test
 *  main.c - Basic Large Stack Performance Test
 *
 *  Copyright 2013 by Citrusleaf, Aerospike Inc.  All rights reserved.
 *  THIS IS UNPUBLISHED PROPRIETARY SOURCE CODE.  THE COPYRIGHT NOTICE
 *  ABOVE DOES NOT EVIDENCE ANY ACTUAL OR INTENDED PUBLICATION.
 */

#include "test.h"
#include "test_counter.h"

// Global Configuration object: holds client config data.
test_config *g_config = NULL;

static char * MOD = "test_main.c::0422";

/** ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
 *  Show Usage
 */
void usage(int argc, char *argv[]) {
    printf("Usage %s:\n", argv[0]);
    printf("   -h host [default 127.0.0.1] \n");
    printf("   -p port [default 3000]\n");
    printf("   -n namespace [default test]\n");
    printf("   -s set [default *all*]\n");
    printf("   -v turn on VERBOSE setting\n");
    printf("   -t Number of Threads to use.\n");
    printf("   -i Number of Test Iterations (per thread).\n");
    printf("   -c Run with the default cluster settings.\n");
    printf("   -e Set Max number of Peek Counts per op.\n");
    printf("   -k Set Max Key Range (0::Max). \n");
}


/** ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
 *  Setup Cluster (this is manual for now)
 */
void setup_cluster( test_config * config ){
    // Build a set of cluster names that we'll connect to in the
    // configuration.
    printf("[ENTER]:Setup Cluster \n");

    config->cluster_count = 4;
    config->cluster_name[0] = "192.168.120.101";
    config->cluster_name[1] = "192.168.120.102";
    config->cluster_name[2] = "192.168.120.103";
    config->cluster_name[3] = "192.168.120.104";

    config->cluster_port[0] = 3000;
    config->cluster_port[1] = 3000;
    config->cluster_port[2] = 3000;
    config->cluster_port[3] = 3000;
}

/** ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
 *  Set up the configuration for the Test Run Routines
 */
int init_configuration (int argc, char *argv[]) {
    static char * meth = "init_configuration()";

    printf("[ENTER]:<%s:%s>: Num Args (%d)\n", MOD, meth, argc );

    g_config = (test_config *)malloc(sizeof(test_config));
    memset(g_config, 0, sizeof(test_config));

    set_config_defaults( g_config );

    INFO("[DEBUG]:<%s:%s>: Num Args (%d) g_config(%p)\n",
            MOD, meth, argc, g_config);

    INFO("[DEBUG]:<%s:%s>: About to Process Args (%d)\n", MOD, meth, argc );
    int optcase;
    while ((optcase = getopt(argc, argv, "ckmh:p:n:s:P:f:v:x:r:t:i:j:")) != -1){
        INFO("[ENTER]:<%s:%s>: Processings Arg(%d)\n", MOD, meth, optcase );
        switch (optcase) {
        case 'h': g_config->host    = strdup(optarg); break;
        case 'p': g_config->port    = atoi(optarg);   break;
        case 'n': g_config->ns      = strdup(optarg); break;
        case 's': g_config->set     = strdup(optarg); break;
        case 'v': g_config->verbose = true;           break;
        case 't': g_config->n_threads = atoi(optarg); break;
        case 'i': g_config->n_iterations = atoi(optarg);   break;
        case 'c': setup_cluster( g_config );         break;
        case 'e': g_config->peek_max = atoi(optarg); break;
        case 'k': g_config->key_max = atoi(optarg);  break;
        default:  usage(argc, argv);                 return(-1);
        }
    }
    return 0;
}

// Invoke the program that runs in the thread.  Pass in the address of
// a simple integer (the logical thread number).
//
// Not sure why this has to be "static", but perhaps that's how the
// functions are invoked for a thread.
static void *run_test(void *o) {
    char user_key[30];
    unsigned int thread_num;
    if( o != NULL ) {
        thread_num = *(unsigned int *) o;
    }  
    
    // Don't need this yet
    // srand( thread_num );
    // unsigned int random_num = rand() % 100;

    sprintf(user_key, "User_%d", thread_num );
    printf(">>>>>>>>>=================<<<<<<<<<<\n");
    printf(">>>>>>>   RUN TEST 1 ::Thread(%u)<<< User key(%s)\n",
                        thread_num, user_key);
    printf(">>>>>>>>>=================<<<<<<<<<<\n");
    run_test1(user_key, thread_num );

    sprintf(user_key, "User_%d", thread_num );
    printf(">>>>>>>>>=================<<<<<<<<<<\n");
    printf(">>>>>>>   RUN TEST 2 ::Thread(%u)<<< User key(%s)\n",
                        thread_num, user_key);
    printf(">>>>>>>>>=================<<<<<<<<<<\n");
    run_test2(user_key, thread_num );
/*
    sprintf(user_key, "User_%d", thread_num );
    printf(">>>>>>>>>=================<<<<<<<<<<\n");
    printf(">>>>>>>   RUN TEST 3 ::Thread(%u)<<< Seed(%u)\n",
                        thread_num, thread_num);
    printf(">>>>>>>>>=================<<<<<<<<<<\n");
    run_test3( thread_num );
*/
    return NULL;
}

// |||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
/**
 * Print out our counters (maybe this belongs in Config?)
 */
void print_counters( uint64_t seconds ) {
    fprintf(stderr, "TEST(FN): Total Keys(%"PRIu64") \n",
        cf_atomic_int_get(g_config->key_counter->val) );

    fprintf(stderr,":: Read Ops(%"PRIu64") Read Vals(%"PRIu64") S(%"PRIu64")\n",
        cf_atomic_int_get(g_config->read_ops_counter->val),
        cf_atomic_int_get(g_config->read_vals_counter->val), seconds );

    fprintf(stderr,":: Read Ops/Sec(%"PRIu64") Read Vals/Sec(%"PRIu64")\n",
        cf_atomic_int_get(g_config->read_ops_counter->val) / seconds,
        cf_atomic_int_get(g_config->read_vals_counter->val)/ seconds );

    fprintf(stderr,":: Write Ops(%"PRIu64") Write Vals(%"PRIu64")S(%"PRIu64")\n",
        cf_atomic_int_get(g_config->write_ops_counter->val),
        cf_atomic_int_get(g_config->write_vals_counter->val), seconds );

    fprintf(stderr,":: Write Ops/Sec(%"PRIu64") Write Vals/Sec(%"PRIu64")\n",
        cf_atomic_int_get(g_config->write_ops_counter->val)/seconds,
        cf_atomic_int_get(g_config->write_vals_counter->val)/ seconds );

    fprintf(stderr,":: Delete Ops(%"PRIu64") Delete Vals(%"PRIu64")S(%"PRIu64")\n",
        cf_atomic_int_get(g_config->delete_ops_counter->val),
        cf_atomic_int_get(g_config->delete_vals_counter->val), seconds );

    fprintf(stderr,":: Delete Ops/Sec(%"PRIu64") Delete Vals/Sec(%"PRIu64")\n",
        cf_atomic_int_get(g_config->delete_ops_counter->val) / seconds,
        cf_atomic_int_get(g_config->delete_vals_counter->val) / seconds );

} // end print_stats()


/** ||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||||
*  This file exercises the Aerospike Interface.
*  For a particular TestSuite, it will exercise various test scripts
*  under a variety of conditions, and measure it while doing so.
*
*  Launch "T" Threads
*  Each Thread will perform a set of operations for each User (record key).
*  Each Set of operations will follow one of the many scripts defined here.
*/
int main(int argc, char **argv) {
static char * meth         = "main()";
    int           rc           = 0;
    int j = 0;
    char * test_name = "LSTACK Test Run::April 29";

    // Run this FIRST THING
    init_configuration( argc, argv );

    INFO("[ENTER]:<%s:%s>: Start in main()\n", MOD, meth );

    INFO("[DEBUG]:<%s:%s>: calling setup_test()\n", MOD, meth );
    if (setup_test( argc, argv )) {
        return 0;
    }

    printf("<< Test Run >> Start (%s)\n", test_name );

    // Start our stopwatch for this test run
    uint64_t start_time = cf_getms();

    // Set up for operation counting;
    g_config->read_ops_counter     = atomic_int_create((uint64_t) 0);
    g_config->read_vals_counter    = atomic_int_create((uint64_t) 0);
    g_config->write_ops_counter    = atomic_int_create((uint64_t) 0);
    g_config->write_vals_counter   = atomic_int_create((uint64_t) 0);
    g_config->delete_ops_counter   = atomic_int_create((uint64_t) 0);
    g_config->delete_vals_counter  = atomic_int_create((uint64_t) 0);
    g_config->key_counter          = atomic_int_create((uint64_t) 0);
    g_config->success_counter      = atomic_int_create((uint64_t) 0);
    g_config->fail_counter         = atomic_int_create((uint64_t) 0);
    
    void *counter_control = start_test_counter_thread(
            g_config->read_ops_counter, g_config->read_vals_counter,
            g_config->write_ops_counter, g_config->write_vals_counter,
            g_config->delete_ops_counter, g_config->delete_vals_counter,
            g_config->key_counter);
    pthread_t slaps[g_config->n_threads];
    // Invoke the "run_test()" method for g_config->n_threads many threads
    for ( j = 0; j < g_config->n_threads; j++) {
        if (pthread_create(&slaps[j], 0, run_test, &j )) {
            INFO("[WARNING]: Thread Create Failed\n");
        }
    }
    for ( j = 0; j < g_config->n_threads; j++) {
        pthread_join(slaps[j], (void *)&rc);
    }

    // End Performance Testing
    stop_test_counter_thread(counter_control);

    uint64_t stop_time = cf_getms();
    uint64_t elapsed_time_ms = stop_time - start_time;
    uint64_t elapsed_time_s = elapsed_time_ms / 1000;


    printf("<< Test Run >> End (%s) \n", test_name );

    fprintf(stderr,"[LSO Test Run] Stop: Total Ops(%d) Time Elapsed (%lu)ms\n",
            (g_config->n_threads * g_config->n_iterations),
            (stop_time - start_time) );

    print_counters( elapsed_time_s );

    printf("CITRUSLEAF STATS Follows ... \n");
    citrusleaf_print_stats();

    shutdown_test();
    exit(0);
} // end main()

