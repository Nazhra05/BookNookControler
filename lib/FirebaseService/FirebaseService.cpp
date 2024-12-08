#include "FirebaseService.h"
#include <ArduinoJson.h>

FirebaseService::FirebaseService(const char *apiKey,
                                 const char *projectId,
                                 const char *userEmail,
                                 const char *userPassword)
    : _apiKey(apiKey),
      _projectId(projectId),
      _userEmail(userEmail),
      _userPassword(userPassword),
      _userAuth(_apiKey, _userEmail, _userPassword, 3000),
      _aClient(_sslClient, getNetwork(_network)),
      _parent(_projectId)
{
    Firebase.printf("Firebase Client v%s\n", FIREBASE_CLIENT_VERSION);
    _sslClient.setInsecure();
}

void FirebaseService::initialize()
{
    initializeApp(_aClient, _app, getAuth(_userAuth), _aResult);
    FirebaseService::authHandler();
    Serial.print("Initializing app..");

    _app.getApp<Firestore::Documents>(_docs);
    _aClient.setAsyncResult(_aResult);
}

void FirebaseService::appLoop()
{
    FirebaseService::authHandler();
    _docs.loop();
}

String FirebaseService::validateBarcode(const char *barcode)
{

    Projection projection(FieldReference("uid"));

    FieldFilter fieldFilter;
    fieldFilter.field(FieldReference("barcode"));
    fieldFilter.op(FieldFilterOperator::EQUAL);
    Values::StringValue data(barcode);
    fieldFilter.value(Values::Value(data));

    ResponseQuery response = FirebaseService::query(projection, "users", fieldFilter);

    if (!response.success)
        return "";

    serializeJsonPretty(response.data, Serial);
    Serial.println();

    return response.data[0]["document"]["fields"]["uid"]["stringValue"];
}

ResponseQuery FirebaseService::query(Projection projection, String collection, FieldFilter filter, byte limit)
{
    ResponseQuery response;

    StructuredQuery query;
    query.select(projection);
    query.from(CollectionSelector(collection, false));
    query.where(Filter(filter));

    if (limit > 0)
        query.limit(limit);

    QueryOptions queryOptions;
    queryOptions.structuredQuery(query);

    query.clear();
    projection.clear();
    filter.clear();

    String payload = _docs.runQuery(_aClient, _parent, "/", queryOptions);
    queryOptions.clear();

    if (FirebaseService::checkError())
    {
        Serial.println("Ada error");
        response.success = false;
        response.message = "Ada error ketika melakukan query";
        return response;
    }

    if (payload.lastIndexOf(']') < 0)
    {
        payload += "]";
    }

    JsonDocument doc;
    DeserializationError error = deserializeJson(doc, payload);

    if (error)
    {
        Serial.print(F("deserializeJson() failed: "));
        Serial.println(error.f_str());
        response.success = false;
        response.message = "deserializeJson() failed";
        return response;
    }

    response.data = doc;
    response.success = true;
    response.message = "Berhasil mengambil Data";

    return response;
}

bool FirebaseService::addHistory(const char *uid, const char *time)
{
    Values::StringValue uidV(uid);
    Values::TimestampValue timeV(time);

    Document<Values::Value> doc("uid", Values::Value(uidV));
    doc.add("time", Values::Value(timeV));

    // format docPath "collectionId/DocumentId", if whant documentId create automatic only set "collectionId"
    String docPath = "History";

    String payload = _docs.createDocument(_aClient, _parent, docPath, DocumentMask(), doc);

    if (checkError())
    {
        Serial.println("Error when create history");
        return false;
    }
    Serial.print(payload);
    Serial.println();

    return true;
}

void FirebaseService::updateData()
{
    PatchDocumentOptions patchOptions(DocumentMask(""), DocumentMask(""), Precondition());
    Values::IntegerValue angka(1000);

    Document<Values::Value> doc("data", Values::Value(angka));

    _docs.patch(_aClient, _parent, "aa/bb", patchOptions, doc, _aResult);

    if (FirebaseService::checkError())
    {
        return;
    }
    printResult(_aResult);
}

void FirebaseService::printResult(AsyncResult &aResult)
{
    if (aResult.isEvent())
    {
        Firebase.printf("Event task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.appEvent().message().c_str(), aResult.appEvent().code());
    }

    if (aResult.isDebug())
    {
        Firebase.printf("Debug task: %s, msg: %s\n", aResult.uid().c_str(), aResult.debug().c_str());
    }

    if (aResult.isError())
    {
        Firebase.printf("Error task: %s, msg: %s, code: %d\n", aResult.uid().c_str(), aResult.error().message().c_str(), aResult.error().code());
    }

    if (aResult.available())
    {
        Firebase.printf("task: %s, payload: %s\n", aResult.uid().c_str(), aResult.c_str());
    }
}

bool FirebaseService::checkError()
{
    if (_aClient.lastError().code() != 0)
    {
        Firebase.printf("Error, msg: %s, code: %d\n", _aClient.lastError().message().c_str(), _aClient.lastError().code());
        return true;
    }
    return false;
}

bool FirebaseService::isReady()
{
    return _app.ready();
}

String FirebaseService::getUid()
{
    return _app.getUid();
}

String FirebaseService::getToken()
{
    return _app.getToken();
}

String FirebaseService::getRefreshToken()
{
    return _app.getRefreshToken();
}

void FirebaseService::authHandler()
{
    // Blocking authentication handler with timeout
    unsigned long ms = millis();
    while (_app.isInitialized() && !_app.ready() && millis() - ms < 120 * 1000)
    {
        JWT.loop(_app.getAuth());
        printResult(_aResult);
    }
}