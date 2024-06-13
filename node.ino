#include <painlessMesh.h>

#define MESH_PREFIX "meshNetwork"
#define MESH_PASSWORD "meshPassword"
#define MESH_PORT 5555

painlessMesh mesh;

String nodeIdStr;
int RSSIValue = 0;

void broadcastRSSI() {
  RSSIValue = WiFi.RSSI();
  String msg = "RSSI:" + nodeIdStr + ":" + String(RSSIValue);
  mesh.sendBroadcast(msg);
}

void receivedCallback(uint32_t from, String &msg) {
  Serial.println("Received from " + String(from) + ": " + msg);
  
  if (msg.startsWith("RSSI:")) {
    int delim = msg.indexOf(':', 5);
    if (delim != -1) {
      String rssiValue = msg.substring(delim + 1);
      mesh.sendSingle(from, "ACK:" + nodeIdStr);
    }
  }
}

void setup() {
  Serial.begin(115200);
  mesh.setDebugMsgTypes(ERROR | STARTUP);
  mesh.init(MESH_PREFIX, MESH_PASSWORD, MESH_PORT);
  mesh.onReceive(&receivedCallback);
  nodeIdStr = String(mesh.getNodeId());
}

void loop() {
  mesh.update();
  broadcastRSSI();
  delay(2000);
}
