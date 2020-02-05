#include <stdlib.h>
#pragma pack(1)
#include "dbInsert.h"
#include "dbRead.h"
#include "dbUpdate.h"

// TODO Insetion on inactive pos
void insertM() {
  if (!validateRecordsAmount(shopsIndices))
    return;
  insertNewShop();
  insertNewShopIndex();
}


// TODO Insetion on inactive pos
void insertS() {
  if (!validateRecordsAmount(employeesIndices))
    return;
  shop mShop = getM();
  if (mShop.isActive) {
    insertNewEmployeeRecord(mShop);
    insertNewEmployeeIndex();
  }
}

employee getNewEmployeeRecord(shop mShop) {
  employee newEmployee = {.id = getRecordsNum(employeesData) + 1,
                          .prevColleagueId = -1,
                          .nextColleagueId = mShop.employeeId,
                          .isActive = true};
  if (mShop.employeeId != -1) {
    employee prevEmployee = getEmployeeByKey(mShop.employeeId);
    prevEmployee.prevColleagueId = newEmployee.id;
  }
  // TODO

  printf("\\\\ Enter credentials of new employee:\n"
         "\\\\ First name: ");
  fflush(stdin);
  gets(newEmployee.firstName);
  printf("\\\\ Last name: ");
  fflush(stdin);
  gets(newEmployee.lastName);
  return newEmployee;
}

bool validateRecordsAmount(dbFiles fileType) {
  const char *fileNames[] = {"Shops.fl", "Shops.ind", "Employees.fl",
                             "Employees.ind"};
  if (getRecordsNum(fileType) >= MAX_RECORDS_AMOUNT) {
    fprintf(stderr, "\n%s cannot store more than 20 records!\n",
            fileNames[fileType]);
    return false;
  }
  return true;
}

keyIndex getNewDataIndex(dbFiles fileType) {
  keyIndex newShopIndex = {.key = getRecordsNum(fileType)};
  if (fileType == shopsData || fileType == shopsIndices)
    newShopIndex.address = getRecordsNum(fileType) * sizeof(shop);
  else
    newShopIndex.address = getRecordsNum(fileType) * sizeof(employee);
  return newShopIndex;
}

void insertNewShop() {
  shop newShop = getNewShopRecord();
  FILE *outputFile = NULL;
  openDbFile(&outputFile, shopsData);
  fseek(outputFile, 0L, SEEK_END);
  fwrite(&newShop, sizeof(shop), 1, outputFile);
  fclose(outputFile);
}

shop getNewShopRecord() {
  shop newShop = {.id = getRecordsNum(shopsData) + 1,
      .employeeId = -1,
      .isActive = true};
  printf("\\\\ New address: ");
  fflush(stdin);
  gets(newShop.address);
  return newShop;
}

void insertNewEmployeeRecord(shop mShop) {
  employee newEmployee = getNewEmployeeRecord(mShop);
  mShop.employeeId = newEmployee.id;
  updateShop(mShop);

  FILE *outputFile = NULL;
  openDbFile(&outputFile, employeesData);
  fseek(outputFile, 0L, SEEK_END);
  fwrite(&newEmployee, sizeof(employee), 1, outputFile);
  fclose(outputFile);
}

void insertNewShopIndex() {
  FILE *outputFile = NULL;
  keyIndex newShopIndex = getNewDataIndex(shopsData);
  int recordNum = newShopIndex.key + 1;
  openDbFile(&outputFile, shopsIndices);
  fwrite(&recordNum, sizeof(int), 1, outputFile);
  fseek(outputFile, 0L, SEEK_END);
  fwrite(&newShopIndex, sizeof(keyIndex), 1, outputFile);
  fclose(outputFile);
}

void insertNewEmployeeIndex() {
  FILE *outputFile = NULL;
  keyIndex newEmployeeIndex = getNewDataIndex(employeesData);
  int recordNum = newEmployeeIndex.key + 1;
  openDbFile(&outputFile, employeesIndices);
  fwrite(&recordNum, sizeof(int), 1, outputFile);
  fseek(outputFile, 0L, SEEK_END);
  fwrite(&newEmployeeIndex, sizeof(keyIndex), 1, outputFile);
  fclose(outputFile);
}