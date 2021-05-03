#ifndef HTTP_H
#define HTTP_H

#define CONTINUE                100
#define SWITCHING_PROTOCALS     101
#define OK                      200
#define CREATED                 201
#define ACCEPTED                202
#define NON_AUTH_INFO           203
#define NO_CONTENT              204
#define RESET_CONTENT           205
#define PARTIAL_CONTENT         206
#define MOVED                   301
#define FOUND                   302
#define NOT_MODIFIED            304
#define TEMP_REDIRECT           307
#define PERM_REDIRECT           308
#define BAD_REQUEST             400
#define UNAUTHORIZED            401
#define FORBIDDEN               403
#define NOT_FOUND               404
#define METHOD_NOT_ALLOWED      405
#define NOT_ACCEPTABLE          406
#define REQUEST_TIMEOUT         408
#define CONFLICT                409
#define GONE                    410
#define LENGTH_REQUIRED         411
#define PAYLOAD_TOO_LARGE       413
#define UNSUPPORTED_MEDIA_TYPE  415
#define TOO_MANY_REQUESTS       429
#define INTERNAL_SERVER_ERROR   500
#define SERVICE_UNAVAILABLE     501
#define VERSION                 "HTTP/1.1"

#endif // HTTP_H
