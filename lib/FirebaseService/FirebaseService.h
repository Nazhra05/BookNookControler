#ifndef FIREBASE_SERVICE_H
#define FIREBASE_SERVICE_H

#include <Arduino.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <FirebaseClient.h>
#include <ResponseQuery.h>
#include <BookChangeResult.h>

class FirebaseService
{
private:
    String _apiKey;
    String _projectId;
    String _userEmail;
    String _userPassword;
    UserAuth _userAuth;
    WiFiClientSecure _sslClient;
    DefaultNetwork _network;
    AsyncResult _aResult;
    AsyncClientClass _aClient;
    FirebaseApp _app;

    Firestore::Documents _docs;
    Firestore::Parent _parent;

    void printResult(AsyncResult &aResult);
    bool checkError();

public:
    FirebaseService(const char *apiKey, const char *projectId, const char *userEmail, const char *userPassword);
    void initialize();
    void appLoop();
    String validateBarcode(const char *barcode);
    ResponseQuery query(Projection projection, String collection, FieldFilter filter, byte limit = 0);
    bool addHistory(const char *uid, BookChangeResult books, const char *borrowTime, const char *returnTime);
    bool updateBookAvailable(BookChangeResult books);
    bool setDoorStatusOpen(const char *uid, const char *timeUpdate);
    String getDoorStatus();
    void updateData();
    bool isReady();
    String getUid();
    String getToken();
    String getRefreshToken();
    void authHandler();
};

#endif
