extern "C" {
#include "user_interface.h"
}

#include "set"
using namespace std;

#define FRAME_TYPE_MANAGEMENT 0
#define FRAME_TYPE_CONTROL 1
#define FRAME_TYPE_DATA 2
#define FRAME_SUBTYPE_PROBE_REQUEST 0x04
#define FRAME_SUBTYPE_PROBE_RESPONSE 0x05
#define FRAME_SUBTYPE_BEACON 0x08
#define FRAME_SUBTYPE_AUTH 0x0b
#define FRAME_SUBTYPE_DEAUTH 0x0c
#define FRAME_SUBTYPE_DATA 0x14
typedef struct framectrl_80211
{
    //buf[0]
    u8 Protocol:2;
    u8 Type:2;
    u8 Subtype:4;
    //buf[1]
    u8 ToDS:1;
    u8 FromDS:1;
    u8 MoreFlag:1;
    u8 Retry:1;
    u8 PwrMgmt:1;
    u8 MoreData:1;
    u8 Protectedframe:1;
    u8 Order:1;
} framectrl_80211,*lpframectrl_80211;

typedef struct w80211_header
{
  struct framectrl_80211 framectrl;
  uint16 duration;
  uint8 rdaddr[6]; // receiver
  uint8 tsaddr[6]; // transmitter
  uint8 bssid[6];
  uint16 number; // sequence number
} w80211header;

typedef struct tagged_parameter
{
  /* SSID parameter */
  uint8 tag_number;
  uint8 tag_length;
} tagged_parameter, *ptagged_parameter;

struct RxControl {
    signed rssi:8;// indicates the signal strength of the package
    unsigned rate:4;
    unsigned is_group:1;
    unsigned:1;
    unsigned sig_mode:2;// indicates whether the packet is a packet 11n, 0 represents a non-11n, 11n nonzero
    unsigned legacy_length:12;// if not 11n package, it represents the length of the package
    unsigned damatch0:1;
    unsigned damatch1:1;
    unsigned bssidmatch0:1;
    unsigned bssidmatch1:1;
    unsigned MCS:7;// if the package is 11n, which represents the modulation coding sequence packets RMS: 0-76
    unsigned CWB:1;//  if the package is 11n, which indicates whether the packet HT40
    unsigned HT_length:16;// if the package is 11n, which represents the length of the package
    unsigned Smoothing:1;
    unsigned Not_Sounding:1;
    unsigned:1;
    unsigned Aggregation:1;
    unsigned STBC:2;
    unsigned FEC_CODING:1;// if the package is 11n, which indicates whether the packet LDPC
    unsigned SGI:1;
    unsigned rxend_state:8;
    unsigned ampdu_cnt:8;
    unsigned channel:4;// indicates where the packet channel
    unsigned:12;
};

struct sniffer_buf{
  struct RxControl rx_ctrl;
  u8 buf[48];// header contains ieee80211
  u16 cnt;// Number of packages
};

class MAC {
  public:
  uint8 addr[6];
};

class Comp {
  public:
  bool operator()(MAC m1, MAC m2) {
    for(int i=0; i<6; i++) {
      if(m1.addr[i] < m2.addr[i]) {
        return true;
      } else if (m1.addr[i] > m2.addr[i]) {
        return false;
      }
    }
    return false;
  }
};

set <MAC, Comp> macs;
uint8 sec_count;

void callback(uint8 *buf, uint16 len) {
    uint16 i;
    uint8_t type;

    struct router_info *info = NULL;
    if (len < sizeof(struct RxControl) + sizeof(struct w80211_header)) {
        return;
    }
    struct sniffer_buf *sniffer = (struct sniffer_buf*)buf;
    buf +=sizeof(struct RxControl);
    struct w80211_header *probe_buf = (struct w80211_header*)buf;

    MAC m1, m2;
    os_memset(m1.addr, 0, 6);
    os_memcpy(m1.addr, probe_buf->rdaddr, 6);
    macs.insert(m1);
    os_memset(m2.addr, 0, 6);
    os_memcpy(m2.addr, probe_buf->tsaddr, 6);
    macs.insert(m2);
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();

  Serial.print(" -> Promisc mode setup ...");
  wifi_set_promiscuous_rx_cb(callback);
  wifi_promiscuous_enable(1);
  Serial.println(" done.");

  Serial.print(" -> Set opmode ...");
  wifi_set_opmode( 0x1 );
  Serial.println(" done.");  
}

void loop()
{
  sec_count++;
  uint8 new_channel = wifi_get_channel() % 12 + 1;
  wifi_set_channel(new_channel);
//  Serial.print("Channel set to ");
//  Serial.println(new_channel);

  // wait one second before changing the channel
  delay(1000);

  if (sec_count % 30 == 0) {
    set <MAC>::iterator iter;
    for( iter = macs.begin(); iter != macs.end(); iter++ ) {
      for(int i=0; i<6; i++) {
        Serial.print(iter->addr[i], HEX);
        Serial.print(':');
      }
      Serial.println();
    }
    Serial.println("--------------------");
  }
}
