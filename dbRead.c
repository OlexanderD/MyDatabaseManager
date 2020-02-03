#include <stdlib.h>
#include <stdbool.h>
#pragma pack(1)
#include "dbRead.h"
#include "dbStructures.h"
#include "dbFilesHandler.h"

void getM() {
    int recordsNum = getRecordsAmount(shopsData);
    keyIndex indices[recordsNum];
    if (!readIndices(indices, recordsNum))
        return;

    int userKey = getKeyFromUser(shopsData);
    shop foundedShop = getShopByKey(indices, recordsNum, userKey);
    if (!foundedShop.isActive) {
        fprintf(stderr, "\nCannot find record in Shop.fl\n");
        return;
    }
    printf("\nId: %d\nAddress: %s\n\n", foundedShop.id, foundedShop.address);
}

// TODO
void getS() {

}

// TODO
void getAll() {

}

int getKeyFromUser(dbFiles fileType) {
    int key;
    printf("Enter key of shop: ");
    fflush(stdin);
    scanf("%d", &key);
    return key;
}

bool readIndices(keyIndex indices[], int size) {
    FILE *shopIndicesFile = NULL;
    openDbFile(&shopIndicesFile, shopsIndices);

    fseek(shopIndicesFile, sizeof(int), SEEK_SET);
    unsigned int readKeysNum = fread(indices, sizeof(keyIndex), MAX_AMOUNT, shopIndicesFile);
    fclose(shopIndicesFile);
    if (size != readKeysNum) {
        fprintf(stderr, "\nCannot properly read all Shop.fl records!\n");
        return false;
    }
    return true;
}

shop getShopByKey(keyIndex indices[], int arrSize, int userKey) {
    shop foundedShop;
    if (userKey - 1 >= arrSize) {
        foundedShop.isActive = false;
    } else {
        unsigned long offset = indices[userKey - 1].address;
        FILE *shopDataFile;
        openDbFile(&shopDataFile, shopsData);
        fseek(shopDataFile, (long) offset, SEEK_SET);
        fread(&foundedShop, sizeof(shop), 1, shopDataFile);
        fclose(shopDataFile);
    }
    return foundedShop;
}