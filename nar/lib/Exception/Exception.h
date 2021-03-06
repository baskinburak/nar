#ifndef NAR_EXCEPTION_H
#define NAR_EXCEPTION_H
#include <stdexcept>
#include <string>

namespace nar {
    namespace Exception {
        class ExcpBase {
            private:
                std::string _message;
            public:
                ExcpBase(const char* mess): _message(mess) {}
                ExcpBase(std::string&  mess): _message(mess) {}
                std::string what() const { return _message; }
        };

        namespace Messaging {
            class MessageNotValid : public nar::Exception::ExcpBase {
                private:
                public:
                    MessageNotValid(const char* mess): ExcpBase(mess) {}
            };
        }

        namespace LowLevelMessaging {
            class Error : public nar::Exception::ExcpBase {
                private:
                public:
                    Error(const char* mess): ExcpBase(mess) {}
            };

            class NoSize : public nar::Exception::ExcpBase {
                private:
                public:
                    NoSize(const char* mess): ExcpBase(mess) {}
            };

            class SizeIntOverflow : public nar::Exception::ExcpBase {
                private:
                public:
                    SizeIntOverflow(const char* mess): ExcpBase(mess) {}
            };

            class FormatError : public nar::Exception::ExcpBase {
                private:
                public:
                    FormatError(const char* mess): ExcpBase(mess) {}
            };

            class ServerSizeIntOverflow : public nar::Exception::ExcpBase {
                private:
                    int _len;
                public:
                    ServerSizeIntOverflow(const char* mess, int len): ExcpBase(mess), _len(len) {}
                    int get_len() { return _len; }
            };
        };

        namespace Authentication {
            class NoSuchUsername: public nar::Exception::ExcpBase {
                private:
                public:
                    NoSuchUsername(const char* mess): ExcpBase(mess) {}
            };

            class WrongTaskString: public nar::Exception::ExcpBase {
                private:
                public:
                    WrongTaskString(const char* mess): ExcpBase(mess) {}
            };
        };

        namespace Peer {
            class LeaveSessionError : public nar::Exception::ExcpBase {
            public:
                LeaveSessionError(const char* mess): ExcpBase(mess){}
            };
        }

        namespace Daemon {
            class ActiveChunkError : public nar::Exception::ExcpBase {
            public:
                ActiveChunkError(const char* mess): ExcpBase(mess){}
            };
            class AuthenticationError : public nar::Exception::ExcpBase {
            public:
                AuthenticationError(const char* mess): ExcpBase(mess){}
            };
        }
        namespace Socket {
            class UnknownType : public nar::Exception::ExcpBase {
                private:
                    char _type;
                public:
                    UnknownType(const char* mess, char type): ExcpBase(mess), _type(type) {}
                    char get_type() const { return _type; }
            };

            class BindError : public nar::Exception::ExcpBase {
                private:
                    unsigned short _port;
                    char _type;
                public:
                    BindError(const char* mess, unsigned short port, char type): ExcpBase(mess), _port(port), _type(type) {}
                    unsigned short get_port() const { return _port; }
                    char get_type() const { return _type; }
            };

            class SystemError : public nar::Exception::ExcpBase {
                public:
                    SystemError(const char* mess): ExcpBase(mess) {}
            };

            class WrongSocketType : public nar::Exception::ExcpBase {
                private:
                    char _type;
                public:
                    WrongSocketType(const char* mess, char type): ExcpBase(mess), _type(type) {}
                    char get_type() const { return _type; }
            };

            class ConnectionError : public nar::Exception::ExcpBase {
                private:
                    unsigned short _port;
                    char _type;
                    std::string _host;
                public:
                    ConnectionError(const char* mess, const std::string& host, const unsigned short port, char type): ExcpBase(mess), _host(host), _port(port), _type(type) {}
                    unsigned short get_port() const { return _port; }
                    char get_type() const { return _type; }
                    std::string get_host() const { return _host; }
            };
        };

        namespace Cryption {
            class AesError : public nar::Exception::ExcpBase {
                public:
                    AesError(const char* mess): ExcpBase(mess) {}
                    AesError(std::string& mess): ExcpBase(mess) {}
            };
            class RsaError : public nar::Exception::ExcpBase {
                public:
                    RsaError(const char* mess): ExcpBase(mess) {}
                    RsaError(std::string& mess): ExcpBase(mess) {}
            };


        };

        class Unknown : public nar::Exception::ExcpBase {
            public:
                Unknown(const char* mess): nar::Exception::ExcpBase(mess) {}
        };

        namespace Packet {
            class PacketLenMatchError : public::nar::Exception::ExcpBase {
                private:
                    int _start;
                    int _len;
                public:
                    PacketLenMatchError(const char* mess, int start, int len): ExcpBase(mess), _start(start), _len(len) {}
                    int get_start() { return _start; }
                    int get_len() { return _len; }
            };

            class NotEndpoint : public nar::Exception::ExcpBase {
                private:
                public:
                    NotEndpoint(const char* mess): ExcpBase(mess) {}
            };

            class NoStreamId : public nar::Exception::ExcpBase {
                private:
                    unsigned short _payload_len;
                public:
                    NoStreamId(const char* mess, unsigned short payload_len): ExcpBase(mess), _payload_len(payload_len) {}
                    unsigned short get_payload_len() { return _payload_len; }
            };
        };

        namespace USocket {

            namespace PacketGenerator {
                class NoMorePacket : public nar::Exception::ExcpBase {
                    private:
                        unsigned long _offset;
                    public:
                        NoMorePacket(const char* mess, unsigned long offset): ExcpBase(mess), _offset(offset) {}
                        unsigned long get_offset() { return _offset; }
                };
            };

            class InactivePeer : public nar::Exception::ExcpBase {
                private:
                public:
                    InactivePeer(const char* mess) : ExcpBase(mess) {}
            };

            class NoAvailablePort : public nar::Exception::ExcpBase {
                private:
                public:
                    NoAvailablePort(const char* mess) : ExcpBase(mess) {}
            };
            class ResolveError : public nar::Exception::ExcpBase {
                private:
                    std::string _ip;
                    unsigned short _port;
                public:
                    ResolveError(const char* mess, std::string ip, unsigned short port): ExcpBase(mess), _ip(ip), _port(port) {}
                    std::string get_ip() { return _ip; }
                    unsigned short get_port() { return _port; }
            };
        };

        namespace DomainError {
            class Negative : public nar::Exception::ExcpBase {
                private:
                    int _value;
                public:
                    Negative(const char* mess, int value): nar::Exception::ExcpBase(mess), _value(value) {}
                    int get_value() const { return _value; }
            };
        };

        namespace Peers {
            class NoValidPeer : public nar::Exception::ExcpBase {
                private:
                public:
                    NoValidPeer(const char* mess): nar::Exception::ExcpBase(mess) {}
            };
        };

        namespace MessageTypes {
            class UnknownStatCode : public nar::Exception::ExcpBase {
                private:
                    int _statcode;
                public:
                    UnknownStatCode(const char* mess, int statcode): nar::Exception::ExcpBase(mess), _statcode(statcode) {}
                    int get_statcode() { return _statcode; }
            };
            class RequestRecvFillError : public nar::Exception::ExcpBase {
                private:
                public:
                    RequestRecvFillError(const char* mess): nar::Exception::ExcpBase(mess){}
            };
            class ResponseRecvFillError : public nar::Exception::ExcpBase {
                private:
                public:
                    ResponseRecvFillError(const char* mess): nar::Exception::ExcpBase(mess){}
            };
            class BadMessageResponseReceive : public nar::Exception::ExcpBase {
                private:
                public:
                    BadMessageResponseReceive(const char* mess): nar::Exception::ExcpBase(mess){}
            };
            class BadMessageRequestReceive : public nar::Exception::ExcpBase {
                private:
                public:
                    BadMessageRequestReceive(const char* mess): nar::Exception::ExcpBase(mess){}
            };
            class BadMessageReceive : public nar::Exception::ExcpBase {
                private:
                public:
                    BadMessageReceive(const char* mess): nar::Exception::ExcpBase(mess){}
            };
            class BadlyConstructedMessageSend : public nar::Exception::ExcpBase {
                private:
                public:
                    BadlyConstructedMessageSend(const char* mess): nar::Exception::ExcpBase(mess){}
            };
            class BadJSONRelatedProblemRequest : public nar::Exception::ExcpBase {
                private:
                public:
                    BadJSONRelatedProblemRequest(const char* mess): nar::Exception::ExcpBase(mess){}
                    BadJSONRelatedProblemRequest(std::string& mess): nar::Exception::ExcpBase(mess){}
            };
            class BadJSONRelatedProblemResponse : public nar::Exception::ExcpBase {
                private:
                public:
                    BadJSONRelatedProblemResponse(const char* mess): nar::Exception::ExcpBase(mess){}
            };
            class UserDoesNotExist : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                    UserDoesNotExist(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };
            class InternalServerError : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                InternalServerError(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };
            class InternalDaemonError : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                InternalDaemonError(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };
            class InternalServerDatabaseError : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                    InternalServerDatabaseError(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };
            class InternalCliError : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                InternalCliError(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };
            class BadRequest : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                    BadRequest(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };
            class ServerSocketAuthenticationError : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                    ServerSocketAuthenticationError(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };

            class PullFileDoesNotExist : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                    PullFileDoesNotExist(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };

            class NoValidPeerPull : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                    NoValidPeerPull(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };

            class NoValidPeerPush : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                    NoValidPeerPush(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };
            class UserNameAlreadyExist : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                    UserNameAlreadyExist(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };
            class ResultStringIsWrong : public nar::Exception::ExcpBase {
                private:
                    int _status_code;
                public:
                    ResultStringIsWrong(const char* mess, int status_code ): nar::Exception::ExcpBase(mess), _status_code(status_code) {}
                    int get_status_code() const { return _status_code; }
            };
        };
        namespace File {
            class CompressError : public nar::Exception::ExcpBase {
                private:
                public:
                    CompressError(const char* mess): nar::Exception::ExcpBase(mess) {}
            };
            class CryptError : public nar::Exception::ExcpBase {
                private:
                public:
                    CryptError(const char* mess): nar::Exception::ExcpBase(mess) {}
            };

            class WriteError : public nar::Exception::ExcpBase {
                private:
                    int _offset;
                    int _write_len;
                public:
                    WriteError(const char* mess, int offset, int write_len): nar::Exception::ExcpBase(mess), _offset(offset), _write_len(write_len) {}
                    int get_offset() const { return _offset; }
                    int get_write_len() const { return _write_len; }
            };
            class ReadError : public nar::Exception::ExcpBase {
                private:
                    int _offset;
                    int _read_len;
                public:
                    ReadError(const char* mess, int offset, int read_len): nar::Exception::ExcpBase(mess), _offset(offset), _read_len(read_len) {}
                    int get_offset() const { return _offset; }
                    int get_read_len() const { return _read_len; }
            };
            class WrongMode : public nar::Exception::ExcpBase {
                private:
                    std::string _mode;
                public:
                    WrongMode(const char* mess, const char* mode): nar::Exception::ExcpBase(mess), _mode(mode) {}
                    std::string get_mode() const { return _mode; }
            };
            class UnrecognizedFileMode : public nar::Exception::ExcpBase {
                private:
                    std::string _mode;
                public:
                    UnrecognizedFileMode(const char* mess, const char* mode): nar::Exception::ExcpBase(mess), _mode(mode) {}
                    std::string get_mode() const { return _mode; }
            };
            class OpenFail : public nar::Exception::ExcpBase {
                private:
                    std::string _file_path;
                public:
                    OpenFail(const char* mess, const char* file_path): nar::Exception::ExcpBase(mess), _file_path(file_path) {}
                    std::string get_file_path() const { return _file_path; }
            };

            class OffsetOutOfBounds : public nar::Exception::ExcpBase {
                private:
                    int _offset;
                public:
                    OffsetOutOfBounds(const char* mess, int offset): nar::Exception::ExcpBase(mess), _offset(offset) {}
                    int get_offset() const { return _offset; }
            };

            class NotOpen : public nar::Exception::ExcpBase {
                public:
                    NotOpen(const char* mess): nar::Exception::ExcpBase(mess) {}
            };

            class NothingToRead : public nar::Exception::ExcpBase {
                private:
                    int _file_len;
                    int _offset;
                    int _len;
                public:
                    NothingToRead(const char* mess, int file_len, int offset, int len): nar::Exception::ExcpBase(mess), _file_len(file_len), _offset(offset), _len(len) {}
                    int get_file_len() const { return _file_len; }
                    int get_offset() const { return _offset; }
                    int get_len() const { return _len; }
            };
        };
    };
};

#endif
