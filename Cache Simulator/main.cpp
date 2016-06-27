//
//  main.cpp
//  Cache Simulator
//
//  Created by Anish on 02/12/15.
//  Copyright (c) 2015 Anish. All rights reserved.
//

#include <iostream>
#include <iomanip>
#include <math.h>

using namespace std;

const int MEM_LINE_SIZE = 16;
const int CACHE_LINE_SIZE = 16;
const int NUM_MEM_LINES = 16;
const int NUM_CACHE_LINES = 8;

int main_memory[NUM_MEM_LINES][MEM_LINE_SIZE];

int read_miss_counter = 0;
int read_hit_counter = 0;
int write_miss_counter = 0;
int write_hit_counter = 0;

struct cacheBlock
{
    int set;
    int valid_bit;
    int tag;
    int data[CACHE_LINE_SIZE];
};


//
// This function initializes the cache
//

void init_cache(cacheBlock cache[], int associativity)
{
    
    int no_of_sets= NUM_CACHE_LINES / associativity;
    //int items_in_each_set= NUM_CACHE_LINES / no_of_sets;
    
    int set_counter=0;
    
    for (int i=0; i < NUM_CACHE_LINES; i++)
    {
        
        cache[i].set = set_counter;
        cache[i].valid_bit = 0;
        cache[i].tag = -99;
        
        for (int j=0; j< CACHE_LINE_SIZE; j++)
        {
            cache[i].data[j]=0;
        }
        
        
        if (associativity==1) {
            set_counter++;
        }
        else if(i % associativity != 0)
        {
            set_counter++;
        }
        
    }
}

//
// This function initializes the main memory.
//

void init_Mem()
{
    int value=0;
    
    for(int i=0; i < NUM_MEM_LINES;i++)
    {
        for (int j=0; j< MEM_LINE_SIZE; j++)
        {
            main_memory[i][j] = j + value;
        }
        value= value + 10;
    }
}

//
// This function prints all current contents of cache.
//

void print_cache(cacheBlock cache[])
{
    
    cout<<"================================ CACHE =================================" << endl;
    for (int i=0; i < NUM_CACHE_LINES; i++)
    {
        cout<<"set = ";
        cout<<cache[i].set;
        cout<<"  ";
        cout<<"v = ";
        cout<<cache[i].valid_bit;
        cout<<"  ";
        cout<<"tag = ";
        cout<<cache[i].tag;
        
        cout<<"      ";
        
        for (int j=0; j< CACHE_LINE_SIZE; j++)
        {
            cout<<setw(5);
            cout<<cache[i].data[j];
            cout<<setw(5);
            
        }
        cout<<endl;
    }
    cout<<"========================================================================" << endl;

}

//
// This function prints all current contents of main memory.
//

void print_mem(int num_mem_lines, int mem_line_size)
{
    
    cout<<"================================ MEMORY ================================" << endl;
    
    for(int i=0; i < num_mem_lines;i++)
    {
        for (int j=0; j< mem_line_size; j++)
        {
            cout<<setw(5);
            cout<<main_memory[i][j];
            cout<<setw(5);
        }
        cout<< endl;
    }
    
    cout<<"========================================================================" << endl;
}


//
// This function calculates the number of sets in this cache based on the associativity given.
//

int calc_number_of_sets(int associativity)
{
    int sets = (NUM_CACHE_LINES / associativity);
    return sets;
}

////
//// this function calculates the set number that any cache line belongs to
////
//
//int calc_my_set_number(int cache_line_num, int num_lines, int associativity)
//{
//    
//}
//
////
//// based on associativity returns the set number in cache mapped to a mem block number
////
//
//int map_mem_block_to_set(int block_num, int associativity)
//{
//    
//}



//
// This function receives the memory block number of interest and calculates
// the cache line number associated with is using the formula for 2-Way Set
// Associative caches. Please note that internal to this function, you determine
// the SET NUMBER of the interest. Then within that set you search for the first
// cache line whose valid bit is 0. This cache line number then will be returned
// by this function.
//
// If no cache lines with valid bit equal to 0 were found, then randomly select one
// of the cache lines to be replaced.
//

int map_mem_block_to_cacheline_for_replacement(int block_num, int associativity, cacheBlock  cache[])
{
    int cache_line_num =0;
    int cache_block_num=0;
    int no_of_sets= calc_number_of_sets(associativity);
    //int items_in_each_set= NUM_CACHE_LINES / no_of_sets;
    
    
    if (associativity==1)
    {
        cache_block_num = block_num % NUM_CACHE_LINES;
        cache_line_num=cache_block_num;
    }
    else
    {
        cache_block_num = (block_num % no_of_sets) * associativity ;
    
        if (cache[cache_block_num].valid_bit==0)
            cache_line_num=cache_block_num;
    
        if (cache[cache_block_num].valid_bit==1 && cache[cache_block_num + 1].valid_bit==0)
            cache_line_num=cache_block_num + 1;
    
        if (cache[cache_block_num].valid_bit==1 && cache[cache_block_num + 1].valid_bit==1)
            cache_line_num= cache_block_num + (rand() % 2);
    }
    
    return cache_line_num;
}

//
// This function loads a block from memory (identified by the block number into
// the appropriate cache line (as per the Set associative cache formula). It also
// sets the valid bit for this cache line to 1, and copies the block number of the
// memory into the tag field.
//

void load_from_mem(int block_num, int block_size, cacheBlock cache[], int associativity)
{
    
    int cache_block_num = map_mem_block_to_cacheline_for_replacement(block_num, associativity, cache) ;
    
    cache[cache_block_num].valid_bit=1;
    cache[cache_block_num].tag=block_num;
    
    for (int i=0; i< MEM_LINE_SIZE; i++) {
        
        cache[cache_block_num].data[i]=main_memory[block_num][i];
        
    }
    
    cout<< endl;
    print_cache(cache);
    
}

//
// Stores a single value to the given offset of a given block in memory.
//

void store_to_mem(int block_num, int offset, int value)
{
    main_memory[block_num][offset]=value;
}


//
// This function writes an entire cache line to the appropriate block in the
// memory associated with it.
//

void write_through_to_mem(int block_num, int block_size, int associativity, cacheBlock cache[])
{
    for(int i=0; i< MEM_LINE_SIZE; i++)
    {
        main_memory[block_num][i]=cache->data[i];
    }
}


//
// Gets a mem block number and determines if that block is already in cache memory
// block number has its tag equal to the block number.
//

int is_cache_hit(int block_num, int associativity, cacheBlock cache[])
{
    
    int cache_line_num= -1;
    
    for (int i=0; i< NUM_CACHE_LINES; i++)
    {
        if(block_num == cache->tag)
        {
            cache_line_num = i;
        }
        
    }
    
    return cache_line_num;
}

//
// This function writes a single value to an offset within a given block number
//

void write_to_cache(int block_num, int offset, int value, cacheBlock cache[], int associativity)
{
    int cache_line_num=is_cache_hit(block_num, associativity, cache);
    
    if((cache_line_num != -1) && (cache_line_num < NUM_CACHE_LINES))
    {
        cache[cache_line_num].data[offset]=value;
        write_through_to_mem(block_num, CACHE_LINE_SIZE, associativity, cache);
    }
    else
    {
        cout<<"Block is not present in Cache!!!" << endl;
    }
}

void cache_performance()
{
    cout<<"========================== CACHE PERFORMANCE ===========================" << endl;
    cout<<"Read Misses = ";
    cout<< read_miss_counter << endl;
    cout<<"Read Hits = ";
    cout<< read_hit_counter << endl;
    cout<<"Write Misses = ";
    cout<< write_miss_counter << endl;
    cout<<"Write Hits = ";
    cout<< write_hit_counter << endl;
    cout<<"========================================================================" << endl;
}


int main(int argc, const char * argv[]) {
    // insert code here...
    
    int mem_block_num;
    int offset, value;
    int cache_block_num;
    int associativity;
    int choice;
    
    bool runFlag = true;
    
    cout << "Welcome to Cache Simulator!!" << endl;
    
    cout << "Set Associativity of this cache: " << endl;
    cin >> associativity;
    
    cout << "Number of sets in this cache: ";
    cout << calc_number_of_sets(associativity) << endl;
    
    cacheBlock my_cache[NUM_CACHE_LINES];
    
    init_cache(my_cache, associativity);
    
    init_Mem();

    cout << endl;
    
    while (runFlag)
    {
        
        cache_performance();
        cout << endl;
        
        print_cache(my_cache);
        cout << endl;
        
        print_mem(NUM_MEM_LINES,MEM_LINE_SIZE);
        cout << endl;

        
        cout << "Enter Choice: " << endl;
        cout << endl;
        cout << "[0] to read from mem" << endl;
        cout << "[1] to write to mem" << endl;
        cout << "[2] to quit" << endl;
        cin >> choice;
        
        switch (choice) {
            case 0:
                
                // Read from memory
                cout << "Enter Block Number: ";
                cin >> mem_block_num;
                cout << endl;
                cout << "Enter offset: ";
                cin >> offset;
                cout << endl;
                
                if((is_cache_hit(mem_block_num, associativity, my_cache) != -1) && (is_cache_hit(mem_block_num, associativity, my_cache) < NUM_CACHE_LINES))
                    read_hit_counter = read_hit_counter + 1;
                else
                    read_miss_counter= read_miss_counter + 1;
                
                load_from_mem(mem_block_num, CACHE_LINE_SIZE, my_cache, associativity);
                
                break;
                
            case 1:
                
                // Write to memory
                cout << "Enter Block Number: ";
                cin >> mem_block_num;
                cout << endl;
                cout << "Enter offset: ";
                cin >> offset;
                cout << endl;
                cout << "Enter Value: ";
                cin >> value;
                cout << endl;

                
                if((is_cache_hit(mem_block_num, associativity, my_cache) != -1) && (is_cache_hit(mem_block_num, associativity, my_cache) < NUM_CACHE_LINES))
                {
                    write_hit_counter = write_hit_counter + 1;
                    write_to_cache(mem_block_num, offset, value, my_cache, associativity);
                }
                else
                {
                    write_miss_counter = write_miss_counter + 1;
                    store_to_mem(mem_block_num, offset, value);
                    //load_from_mem(mem_block_num, CACHE_LINE_SIZE, my_cache, associativity);
                }
                break;
                
            case 2:
                
                // Quit
                runFlag = false;
                break;
                
            default:
                break;
        }
    
    }
    return 0;
}


