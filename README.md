# NAR
A distributed file cloud
## Project Structure
- design: UMLs that describes the designs
  - nar-node: Software design of nar-nodes.
  - nar-server: Software design of nar-server.
- nar: Code of the project
  - lib: Libraries that are external and internal
  - narnode: Code of narnodes.
  - narserver: Code of narserver.
- tests: Test scripts.




constantlar full büyük harf olsun. farklı kelimeler alttan tireyle ayrılsın
işte tab yerine 4 space atalım
preferencesta şeyyapabilirsin
tab atınca 4 space olsun diye
topluca yazıyom hepsini dur
- Class isimleri büyük harfle başlar. Sonraki her kelime camel case yazılır
- Kanatlı parantezler ait olduğu yapının sağından başlar
    Örnek:
        class Naber {
        };

        void calc() {
        }

        for(int i=0; i<100; i++) {
        }

- Noktalı virgül, virgül vs. gibi noktalama işaretlerinden sonra 1 boşluk bırakılır.
- Indentation için 4 space kullanılır.
- Fonksiyon isimleri küçük harfle yazılır. Farklı kelimeler alttan tire ile ayrılır.
- Classlarda private variableların sonunda _ bulunur.
- Constantlar komple büyük harf yazılır. Farklı kelimeler _ ile ayrılır.
- Döngü değişkenleri döngüden hemen önce initialize edilir.
- if-else yapıları örnekteki gibi yapılır:

    if(cond) {
    } else if(cond2) {
    } else {
    }



 For Example
/**
* Class that is responsible for
* handling it all
*
* @author: baskin
* @privar: var1, int, holds the fucking thing
* @privar: var2, char, hold the fuck fuck
* @pubvar: var3, int, naber asdasdfaf
* @tested: no
* @todo: hede hude
**/
class RCP {
private:
int var1;
char var2;
public:
int var3;
/**
* A function that does it all
*
* @author: baskin
* @param: naber, int, biseyler
* @param: iyidir, int, comment
* @todo: implement etmek lazım
**/
void f(int naber, int iyidir);
};


## Error Codes for Messaging
**If your error is not here, add it yourself**
* //3xx//: Request related error
    * 301: Format error [NOT JSON, non existing key etc.]
    * 302: Domain error [At least one of the fields does not have required domain]
* //4xx//: Database related error
    * 401: database findDirectoryId(user.user_name,target_dir) in mkdir
    * 402: database findDirectoryId is -1 returned in mkdir
    * 403: nextDirectoryId error or insertDirectory error in mkdir
    * 404: insertDirectoryTo error in mkdir
* //5xx//: Algorithmic error
    * 501: AesCryptor unexpected server error
    * 502: RsaCryptor unexpected server error
    * 503: base64 unexpected server error
* //6xx//: Connection errors
    * 601: Cannot connect to the server [Daemon to CLI]
    * 602: Low level messaging error [send_message and get_message in utilities]
    * 603: Server sent bad message [Bad request or bad response]
    * 604: Server connection broken [Daemon to CLI]
* //7xx//: Normal workflow errors [Errors like not able to authenticate]
    * 701: No such username [Server sends to daemon]
    * 702: Cannot authenticate user [Daemon sends to CLI]
    * 703: Wrong decrypted task string [Server sends to daemon]
    * 704: Cannot open file [Daemon sends to CLI]
    * 705: Cannot open a tempfile [Daemon sends to CLI]
    * 706: Compression error
    * 707: Not enough valid peers to push your file [Server sends to daemon, daemon sends to CLI]
    * 708: Cryption error
    * 799: Peer down
* //900//: Unknown error
