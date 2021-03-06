/**
 * Copyright (C) 2007 Doug Judd (Zvents, Inc.)
 *
 * This file is part of Hypertable.
 *
 * Hypertable is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or any later version.
 *
 * Hypertable is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
 * 02110-1301, USA.
 */

#include "Common/Compat.h"
#include "Common/HashMap.h"

#include "Error.h"

using namespace Hypertable;

namespace {
  struct ErrorInfo {
    int          code;
    const char  *text;
  };

  ErrorInfo error_info[] = {
    { Error::UNPOSSIBLE,                  "But that's unpossible!" },
    { Error::EXTERNAL,                    "External error" },
    { Error::OK,                          "HYPERTABLE ok" },
    { Error::PROTOCOL_ERROR,              "HYPERTABLE protocol error" },
    { Error::REQUEST_TRUNCATED,           "HYPERTABLE request truncated" },
    { Error::RESPONSE_TRUNCATED,          "HYPERTABLE response truncated" },
    { Error::REQUEST_TIMEOUT,             "HYPERTABLE request timeout" },
    { Error::LOCAL_IO_ERROR,              "HYPERTABLE local i/o error" },
    { Error::BAD_ROOT_LOCATION,           "HYPERTABLE bad root location" },
    { Error::BAD_SCHEMA,                  "HYPERTABLE bad schema" },
    { Error::INVALID_METADATA,            "HYPERTABLE invalid metadata" },
    { Error::BAD_KEY,                     "HYPERTABLE bad key" },
    { Error::METADATA_NOT_FOUND,          "HYPERTABLE metadata not found" },
    { Error::HQL_PARSE_ERROR,             "HYPERTABLE HQL parse error" },
    { Error::FILE_NOT_FOUND,              "HYPERTABLE file not found" },
    { Error::BLOCK_COMPRESSOR_UNSUPPORTED_TYPE,  "HYPERTABLE block compressor unsupported type" },
    { Error::BLOCK_COMPRESSOR_INVALID_ARG,       "HYPERTABLE block compressor invalid arg" },
    { Error::BLOCK_COMPRESSOR_TRUNCATED,         "HYPERTABLE block compressor block truncated" },
    { Error::BLOCK_COMPRESSOR_BAD_HEADER,        "HYPERTABLE block compressor bad block header" },
    { Error::BLOCK_COMPRESSOR_BAD_MAGIC,         "HYPERTABLE block compressor bad magic string" },
    { Error::BLOCK_COMPRESSOR_CHECKSUM_MISMATCH, "HYPERTABLE block compressor block checksum mismatch" },
    { Error::BLOCK_COMPRESSOR_INFLATE_ERROR,     "HYPERTABLE block compressor inflate error" },
    { Error::BLOCK_COMPRESSOR_INIT_ERROR,        "HYPERTABLE block compressor initialization error" },
    { Error::TABLE_DOES_NOT_EXIST,               "HYPERTABLE table does not exist" },
    { Error::PARSE_ERROR,                        "HYPERTABLE parse error" },
    { Error::CONNECT_ERROR_MASTER,               "HYPERTABLE Master connect error" },
    { Error::CONNECT_ERROR_HYPERSPACE,           "HYPERTABLE Hyperspace connect error" },
    { Error::TOO_MANY_COLUMNS,            "HYPERTABLE too many columns" },
    { Error::BAD_DOMAIN_NAME,             "HYPERTABLE bad domain name" },
    { Error::FAILED_EXPECTATION,          "HYPERTABLE failed expectation" },
    { Error::MALFORMED_REQUEST,           "HYPERTABLE malformed request" },
    { Error::COMM_NOT_CONNECTED,          "COMM not connected" },
    { Error::COMM_BROKEN_CONNECTION,      "COMM broken connection" },
    { Error::COMM_CONNECT_ERROR,          "COMM connect error" },
    { Error::COMM_ALREADY_CONNECTED,      "COMM already connected" },
    { Error::COMM_REQUEST_TIMEOUT,        "COMM request timeout" },
    { Error::COMM_SEND_ERROR,             "COMM send error" },
    { Error::COMM_RECEIVE_ERROR,          "COMM receive error" },
    { Error::COMM_POLL_ERROR,             "COMM poll error" },
    { Error::COMM_CONFLICTING_ADDRESS,    "COMM conflicting address" },
    { Error::COMM_SOCKET_ERROR,           "COMM socket error" },
    { Error::COMM_BIND_ERROR,             "COMM bind error" },
    { Error::COMM_LISTEN_ERROR,           "COMM listen error" },
    { Error::DFSBROKER_BAD_FILE_HANDLE,   "DFS BROKER bad file handle" },
    { Error::DFSBROKER_IO_ERROR,          "DFS BROKER i/o error" },
    { Error::DFSBROKER_FILE_NOT_FOUND,    "DFS BROKER file not found" },
    { Error::DFSBROKER_BAD_FILENAME,      "DFS BROKER bad filename" },
    { Error::DFSBROKER_PERMISSION_DENIED, "DFS BROKER permission denied" },
    { Error::DFSBROKER_INVALID_ARGUMENT,  "DFS BROKER invalid argument" },
    { Error::DFSBROKER_INVALID_CONFIG,    "DFS BROKER invalid config value" },
    { Error::HYPERSPACE_IO_ERROR,         "HYPERSPACE i/o error" },
    { Error::HYPERSPACE_CREATE_FAILED,    "HYPERSPACE create failed" },
    { Error::HYPERSPACE_FILE_NOT_FOUND,   "HYPERSPACE file not found" },
    { Error::HYPERSPACE_ATTR_NOT_FOUND,   "HYPERSPACE attribute not found" },
    { Error::HYPERSPACE_DELETE_ERROR,     "HYPERSPACE delete error" },
    { Error::HYPERSPACE_BAD_PATHNAME,     "HYPERSPACE bad pathname" },
    { Error::HYPERSPACE_PERMISSION_DENIED,"HYPERSPACE permission denied" },
    { Error::HYPERSPACE_EXPIRED_SESSION,  "HYPERSPACE expired session" },
    { Error::HYPERSPACE_FILE_EXISTS,      "HYPERSPACE file exists" },
    { Error::HYPERSPACE_IS_DIRECTORY,     "HYPERSPACE is directory" },
    { Error::HYPERSPACE_INVALID_HANDLE,   "HYPERSPACE invalid handle" },
    { Error::HYPERSPACE_REQUEST_CANCELLED,"HYPERSPACE request cancelled" },
    { Error::HYPERSPACE_MODE_RESTRICTION, "HYPERSPACE mode restriction" },
    { Error::HYPERSPACE_ALREADY_LOCKED,   "HYPERSPACE already locked" },
    { Error::HYPERSPACE_LOCK_CONFLICT,    "HYPERSPACE lock conflict" },
    { Error::HYPERSPACE_NOT_LOCKED,       "HYPERSPACE not locked" },
    { Error::HYPERSPACE_BAD_ATTRIBUTE,    "HYPERSPACE bad attribute" },
    { Error::HYPERSPACE_BERKELEYDB_ERROR, "HYPERSPACE Berkeley DB error" },
    { Error::HYPERSPACE_DIR_NOT_EMPTY,    "HYPERSPACE directory not empty" },
    { Error::HYPERSPACE_BERKELEYDB_DEADLOCK, "HYPERSPACE Berkeley DB deadlock" },
    { Error::MASTER_TABLE_EXISTS,         "MASTER table exists" },
    { Error::MASTER_BAD_SCHEMA,           "MASTER bad schema" },
    { Error::MASTER_NOT_RUNNING,          "MASTER not running" },
    { Error::MASTER_NO_RANGESERVERS,      "MASTER no range servers" },
    { Error::RANGESERVER_GENERATION_MISMATCH,  "RANGE SERVER generation mismatch" },
    { Error::RANGESERVER_RANGE_ALREADY_LOADED, "RANGE SERVER range already loaded" },
    { Error::RANGESERVER_RANGE_MISMATCH,       "RANGE SERVER range mismatch" },
    { Error::RANGESERVER_NONEXISTENT_RANGE,    "RANGE SERVER non-existent range" },
    { Error::RANGESERVER_OUT_OF_RANGE,         "RANGE SERVER out of range" },
    { Error::RANGESERVER_RANGE_NOT_FOUND,      "RANGE SERVER range not found" },
    { Error::RANGESERVER_INVALID_SCANNER_ID,   "RANGE SERVER invalid scanner id" },
    { Error::RANGESERVER_SCHEMA_PARSE_ERROR,   "RANGE SERVER schema parse error" },
    { Error::RANGESERVER_SCHEMA_INVALID_CFID,  "RANGE SERVER invalid column family id" },
    { Error::RANGESERVER_INVALID_COLUMNFAMILY, "RANGE SERVER invalid column family" },
    { Error::RANGESERVER_TRUNCATED_COMMIT_LOG, "RANGE SERVER truncated commit log" },
    { Error::RANGESERVER_NO_METADATA_FOR_RANGE, "RANGE SERVER no metadata for range" },
    { Error::RANGESERVER_SHUTTING_DOWN,        "RANGE SERVER shutting down" },
    { Error::RANGESERVER_CORRUPT_COMMIT_LOG,   "RANGE SERVER corrupt commit log" },
    { Error::RANGESERVER_UNAVAILABLE,          "RANGE SERVER unavailable" },
    { Error::RANGESERVER_TIMESTAMP_ORDER_ERROR,"RANGE SERVER supplied timestamp is not strictly increasing" },
    { Error::RANGESERVER_ROW_OVERFLOW,         "RANGE SERVER row overflow" },
    { Error::RANGESERVER_TABLE_NOT_FOUND,      "RANGE SERVER table not found" },
    { Error::RANGESERVER_BAD_SCAN_SPEC,        "RANGE SERVER bad scan specification" },
    { Error::HQL_BAD_LOAD_FILE_FORMAT,         "HQL bad load file format" },
    { Error::METALOG_BAD_RS_HEADER, "METALOG bad range server metalog header" },
    { Error::METALOG_BAD_M_HEADER,  "METALOG bad master metalog header" },
    { Error::METALOG_ENTRY_TRUNCATED,   "METALOG entry truncated" },
    { Error::METALOG_CHECKSUM_MISMATCH, "METALOG checksum mismatch" },
    { Error::METALOG_ENTRY_BAD_TYPE, "METALOG bad entry type" },
    { Error::METALOG_ENTRY_BAD_ORDER, "METALOG entry out of order" },
    { Error::SERIALIZATION_INPUT_OVERRUN, "SERIALIZATION input buffer overrun" },
    { Error::SERIALIZATION_BAD_VINT,      "SERIALIZATION bad vint encoding" },
    { Error::SERIALIZATION_BAD_VSTR,      "SERIALIZATION bad vstr encoding" },
    { 0, 0 }
  };

  typedef hash_map<int, const char *>  TextMap;

  TextMap &build_text_map() {
    TextMap *map = new TextMap();
    for (int i=0; error_info[i].text != 0; i++)
      (*map)[error_info[i].code] = error_info[i].text;
    return *map;
  }

  TextMap &text_map = build_text_map();

} // local namespace

const char *Error::get_text(int error) {
  const char *text = text_map[error];
  if (text == 0)
    return "ERROR NOT REGISTERED";
  return text;
}

namespace Hypertable {

std::ostream &operator<<(std::ostream &out, const Exception &e) {
  out <<"Hypertable::Exception: "<< e.what() <<" - "
      << Error::get_text(e.code());

  if (e.line()) {
    out <<"\n\tat "<< e.func() <<" ("<< e.file() <<':'<< e.line() <<')';
  }

  int prev_code = e.code();

  for (Exception *prev = e.prev; prev; prev = prev->prev) {
    out <<"\n\tat "<< (prev->func() ? prev->func() : "-") <<" ("
        << (prev->file() ? prev->file() : "-") <<':'<< prev->line() <<"): "
        << prev->what();

    if (prev->code() != prev_code) {
      out <<" - "<< Error::get_text(prev->code());
      prev_code = prev->code();
    }
  }
  return out;
}

} // namespace Hypertable
