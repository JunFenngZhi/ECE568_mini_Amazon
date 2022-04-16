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
bool checkInventory(connection * C, int itemId, int itemAmount, int whID){
    //create nontransaction object for SELECT operation
    nontransaction N(*C);

    // create sql statement, we need to select item amount from inventory table
    stringstream sql;  
    sql << "SELECT ITEMAMOUNT FROM INVENTORY WHERE "
            "ITEMID= " << itemId << "AND WHID= " << whID <<";";

    // execute sql statement and get the result set    
    result InventoryRes( N.exec(sql.str()));

    // we need to get inventory item amount from result R
    int inventoryAmt = InventoryRes[0][0].as<int>();

    // we compare inventory amt and item amount 
    if(inventoryAmt >= itemAmount){
        return true;
    } else{
        return false;
    }
    
}
