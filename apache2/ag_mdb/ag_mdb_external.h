#ifndef _AG_MDB_EXTERNAL_HEADER
#define _AG_MDB_EXTERNAL_HEADER

#ifdef __cplusplus
extern "C" {
#endif

/**
 **========================================================
 ** AG Memory Database Status Definition
 **========================================================
 */

#define AGMDB_SUCCESS 0        // function return status: success
#define AGMDB_FAIL -1          // function return status: fail (for functions with return type of int)
#define AGMDB_FAIL_P (PTR_VOID)-1 // function return status: fail (for functions with return type of pointer)

/**
 **========================================================
 ** AG Memory Database Handler Definition
 **========================================================
 */

/**
 ** Handler of AG-MDB, which contains a block of shared momory and a set of semaphores.
 ** @param shm_base: The base address of the shared memory.
 ** @param sem_id: The identifier of semaphore set.
 */ 
struct agmdb_handler{
    const void* shm_base;
    void* db_lock;
};

/**
 **========================================================
 ** AG Memory Database Lock API
 **========================================================
 */

/**
 ** Get a shared lock for read only.
 ** @param dbm: the database you want to lock.
 ** return: AGMDB_SUCCESS if successfully created or AGMDB_FAIL if failed.
 */
int AGMDB_getSharedLock(struct agmdb_handler *dbm);

/**
 ** Get a exclusive lock for read and write.
 ** @param dbm: the database you want to lock.
 ** return: AGMDB_SUCCESS if successfully created or AGMDB_FAIL if failed.
 */
int AGMDB_getExclusiveLock(struct agmdb_handler *dbm);

/**
 ** Free a shared lock that you have got before.
 ** @param dbm: the database you want to return the lock.
 ** return: AGMDB_SUCCESS if successfully created or AGMDB_FAIL if failed.
 */
int AGMDB_freeSharedLock(struct agmdb_handler *dbm);

/**
 ** Free a exclusive lock that you have got before.
 ** @param dbm: the database you want to return the lock.
 ** return: AGMDB_SUCCESS if successfully created or AGMDB_FAIL if failed.
 */
int AGMDB_freeExclusiveLock(struct agmdb_handler *dbm);

/**
 **========================================================
 ** AG Memory Database Operation API
 **========================================================
 */

/**
 ** Open a database with given name, and intialize the agmdb handler for further operation.
 ** If the database doesn't exist, a new database will be created.
 ** @param dbm: a created sturcture to save the database information.
 ** @param db_name: the unique identifier of a database.
 ** @param db_name_length: the length of the unique_name.
 ** @param entry_num: The maximum number of entries in the database.
 ** return: AGMDB_SUCCESS if successfully created or AGMDB_FAIL if failed.
 */
int AGMDB_openDB(struct agmdb_handler* dbm, const char* db_name, int db_name_length, unsigned int entry_num);

/**
 ** Close and destroy a database. 
 ** @param dbm: the database you want to destroy.
 ** return: AGMDB_SUCCESS if successfully destroyed or AGMDB_FAIL if failed.
 */
int AGMDB_destroyDB(struct agmdb_handler *dbm);

/**
 ** Close a database, but does not destroy it. 
 ** @param dbm: the database you want to close.
 ** return: AGMDB_SUCCESS if successfully closed or AGMDB_FAIL if failed.
 */
int AGMDB_closeDB(struct agmdb_handler *dbm);

/**
 ** Remove all entries in a database.
 ** You have to get EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to modify.
 ** return: AGMDB_SUCCESS if success
 **      or AGMDB_FAIL if failed.
 */
int AGMDB_cleanDB(struct agmdb_handler *dbm);

/**
 ** Reset the expiration time of the database.
 ** You have to get EXCLUSIVE LOCK of the database before calling this function.
 ** Default expiration time is 3600 seconds.
 ** @param expire_time: how many seconds all entries in database will be kept. 
 */
int AGMDB_setExpireTime(struct agmdb_handler *dbm, unsigned int expire_time); 
 
/**
 ** Get the value from a database with the given key.
 ** You have to get SHARED or EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to read.
 ** @param key: the key string.
 ** @param key_len: the length of key string.
 ** @param buffer: An created buffer to save the value.
 ** @param buffer_len: the length of buffer array.
 ** return: Length of value if the key is fetched or not in the database (buffer will be NULL and return value will be 0 if key does not exist)
 **      or AGMDB_FAIL if failed (cannot get access to database, or the buffer is not long enough).
 */
int AGMDB_get(struct agmdb_handler *dbm, const char* key, int key_len, char* buffer, int buffer_len);

/**
 ** Set the value into database with given key.
 ** You have to get EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to modify.
 ** @param key: the key string.
 ** @param key_len: the length of key string.
 ** @param value: the value string.
 ** @param value: the length of value string.
 ** return: AGMDB_SUCCESS if successfully set
 **      or AGMDB_FAIL if failed.
 */
int AGMDB_set(struct agmdb_handler *dbm, const char* key, int key_len, const char* value, int value_len);

/**
 ** Delete an entry with a given key in a database.
 ** You have to get EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to modify.
 ** @param key: the key string.
 ** @param key_len: the length of key string.
 ** return: AGMDB_SUCCESS if the key is deleted or not in the database
 **      or AGMDB_FAIL if failed.
 */
int AGMDB_delete(struct agmdb_handler *dbm, const char* key, int key_len);

/**
 ** Remove all staled entries.
 ** You have to get EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to remove the stale entries.
 ** return: AGMDB_SUCCESS if success
 **      or AGMDB_FAIL if failed.
 */
int AGMDB_removeStale(struct agmdb_handler *dbm);

/**
 **========================================================
 ** AG Memory Database Debug API
 **========================================================
 */

/**
 ** Get the number of keys in a database.
 ** You have to get SHARED or EXCLUSIVE LOCK of the database before calling this function.
 ** @param dbm: the database you want to read.
 ** return: number of keys in the database
 **      or AGMDB_FAIL if failed.
 */
unsigned int AGMDB_getKeyNum(struct agmdb_handler *dbm);

/**
 ** Use AGMDB's hash function to hash a string into an integer.
 ** @param key: the key string.
 ** @param key_len: the length of the key string.
 ** @param hash_nums: the range of hash.
 ** return: hash result if success
 **      or AGMDB_FAIL if failed.
 */
int AGMDB_getHashValue(const char* key, int key_len, int hash_nums);

#ifdef __cplusplus
}
#endif

#endif

