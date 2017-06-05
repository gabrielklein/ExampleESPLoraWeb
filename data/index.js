$(document).ready(function() {

  setupWebsocket();

  $("#send").click(function() {
    ws.send($("#mess").val());
  });

});

// Just a simple id
var mesId = 0;

// Variable for websocket
var ws;

/**
 * Add a message to the logs
 */
function mess(mes) {
  mesId++;
  var m = $("#recv").text();
  var d = new Date();
  $("#recv").text("[" + mesId + " " + d.getHours() + ":" + d.getMinutes() + ":" + d.getSeconds() + "] " + mes + "\n" + m);
}

/**
 * Initialize websockets
 */
function setupWebsocket() {

  var n = window.location.hostname;
  //n = "10.0.0.36";
  mess("! Connecting to " + n);
  ws = new WebSocket("ws://" + n + ":81/");

  if ("WebSocket" in window) {
    ws.onopen = function() {
      mess("! Connected to WebSocket");
    };

    ws.onmessage = function(evt) {
      var received_msg = evt.data;
      mess("Received: " + received_msg);
    };

    ws.onclose = function() {
      mess("! WebSocket closed");
      setupWebsocket();
    };
  } else {
    mess("! WebSocket is not available");
  }
}
