//#include "main.h"
#include "lfs.h"
#include "w25qxx.h"
#include "lfs_init.h"



int littlefs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);
int littlefs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);
int littlefs_erase(const struct lfs_config *c, lfs_block_t block);
int littlefs_sync(const struct lfs_config *c);

struct lfs_config littlefs_config = {
    // block device operations
    .read  = littlefs_read,
    .prog  = littlefs_prog,
    .erase = littlefs_erase,
    .sync  = littlefs_sync,

    // block device configuration
/*     .read_size = 256,
    .prog_size = 256,
    .block_size = 4096,
    .block_count = 256,
    .cache_size = 256,
    .lookahead_size = 8,
    .block_cycles = 100, */
};


lfs_t littlefs;

int w25qxx_littlefs_init() {
	//LFS_DBG("LittleFS Init");

    w25q32_init_t* w25qxx = w25qxx_getStruct();
    if(!w25qxx){
        return -1;
    }

    littlefs_config.block_size = w25qxx->block_size;
    littlefs_config.block_count = w25qxx->block_count;

    littlefs_config.read_size = 256;
    littlefs_config.prog_size = 256;
    littlefs_config.cache_size = 256;
    littlefs_config.lookahead_size = 8;
    littlefs_config.block_cycles = 100;

	int err = lfs_mount(&littlefs, &littlefs_config);

    // reformat if we can't mount the filesystem
    // this should only happen on the first boot
    if (err) {
        lfs_format(&littlefs, &littlefs_config);
        lfs_mount(&littlefs, &littlefs_config);
    }

    return 0;

}

int littlefs_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size) {
	//LFS_DBG("LittleFS Read b = 0x%04lx o = 0x%04lx s = 0x%04lx", block, off, size);
	
    w25q32_init_t* w25qxx = w25qxx_getStruct();
    if(!w25qxx){
        return -1;
    }
    int offset = 0;

    //TODO memset
    while(offset < size){
        if(!w25qxx_readByte((uint8_t*)buffer + offset, block * w25qxx->sector_size + off)){
            return -1;
        }
        ++offset;
    }
    
	return offset;
}

int littlefs_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size) {
	//LFS_DBG("LittleFS Prog b = 0x%04lx o = 0x%04lx s = 0x%04lx", block, off, size);
	
    w25q32_init_t* w25qxx = w25qxx_getStruct();
    if(!w25qxx){
        return -1;
    }
    int offset = 0;

    //TODO memset
    while(offset < size){
        if(!w25qxx_writeByte((uint8_t*)buffer + offset, block * w25qxx->sector_size + off)){
            return -1;
        }
        ++offset;
    }
    
    //if (w25qxx_write(w25qxx_handle, block * w25qxx_handle->sector_size + off, (void *)buffer, size) != W25QXX_Ok) return -1;
	return offset;
}

int littlefs_erase(const struct lfs_config *c, lfs_block_t block) {
	//LFS_DBG("LittleFS Erase b = 0x%04lx", block);
	
    w25q32_init_t* w25qxx = w25qxx_getStruct();
    if(!w25qxx){
        return -1;
    }

    if(!w25qxx_eraseBlock(block * w25qxx->sector_size)){
        return -1;
    }
    
    //if (w25qxx_erase(w25qxx_handle, block * w25qxx_handle->sector_size, w25qxx_handle->sector_size) != W25QXX_Ok) return -1;
	return 0;
}

int littlefs_sync(const struct lfs_config *c) {
	//LFS_DBG("LittleFS Sync");
	return 0;
}
