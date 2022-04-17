#include "sql_function.h"
#include "exception.h"


/*
    read sql command from the file and then create tabel using connection *C.
    If fails, it will throw exception.
*/
void createTable(connection* C, string fileName) {
    cout<<"create new Tables..."<<endl;
    string sql;
    ifstream ifs(fileName.c_str(), ifstream::in);
    if (ifs.is_open() == true) {
        string line;
        while (getline(ifs, line))
            sql.append(line);
    } else {
        throw MyException("fail to open file.");
    }

    work W(*C);
    W.exec(sql);
    W.commit();
}

/*
    Drop all the table in the DataBase. Using for test.
*/
void dropAllTable(connection* C) {
    work W(*C);
    string sql =
        "DROP TABLE IF EXISTS symbol;DROP TABLE IF EXISTS account;DROP TABLE "
        "IF EXISTS orders;";

    W.exec(sql);
    W.commit();
    cout << "Drop all the existed table..." << endl;
}


/*
    Check the order item amount in the inventory table, return boolean
    True means: enough inventory, False means: not enough inventory
*/
bool checkInventory(connection * C, int itemId, int itemAmount, int whID, int & version){
    //create nontransaction object for SELECT operation
    nontransaction N(*C);

    // create sql statement, we need to select item amount from inventory table
    stringstream sql;  
    sql << "SELECT ITEMAMOUNT, VERSION FROM INVENTORY WHERE "
            "ITEMID= " << itemId << "AND WHID= " << whID <<";";

    // execute sql statement and get the result set    
    result InventoryRes( N.exec(sql.str()));

    // we need to get inventory item amount from result R
    int inventoryAmt = InventoryRes[0][0].as<int>();
    // get the version from the table and change it
    version = InventoryRes[0][1].as<int>();

    // we compare inventory amt and item amount 
    if(inventoryAmt >= itemAmount){
        return true;
    } else{
        return false;
    }
    
}


void saveOrderInDB(connection* C, const Order* order) {



}


/*
    add inventory of the product in the warehouse and update the version id of the inventory
*/
void addInventory(connection * C, int whID, int count, int productId) {
    work W(*C);
    stringstream sql;

    sql << "UPDATE INVENTORY set ITEMAMOUNT = INVENTORY.ITEMAMOUNT+" << count << 
    ", VERSION = INVENTORY.VERSION+1" << " WHERE ITEMID= " << productId << "AND WHID= " << whID <<";";

    W.exec(sql.str());
    W.commit();    
}


/**
 * @brief update specific order status to be 'packed'
 * 
 */
void updatePacked(connection * C, int packageId) {
    // work W(*C);
    // stringstream sql;
    // sql << "UPDATE ORDER set STATUS= " << W.quote("packed") << "WHERE PACKID= " << packageId << 


}


void decreaseInventory(connection * C, int whID, int count, int productId, int version) {
    work W(*C);
    stringstream sql;

    sql << "UPDATE INVENTORY set ITEMAMOUNT = INVENTORY.ITEMAMOUNT+" << count << 
    ", VERSION = INVENTORY.VERSION+1" << " WHERE ITEMID= " << productId << " AND WHID= " << whID << " AND VERSION= "<< version << ";";

    W.exec(sql);
    W.commit();
}