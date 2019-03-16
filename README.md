# openSaysMe
<!--<h1>- OpenSaysMe -</h1>-->
WiFi enabled ESP8266 garage door opener using the Wemos D1 Mini and 2 relays

<b>Description:</b><br>
Garage door opener with basic HTML web page with 2 buttons that send GET request <br>
to ESP8266 board to run functions. Each GET request has its own function that <br>
is called aswell as a 404 error page. Get URLs can be called directly without <br>
going through the web interface. 

<b>Variables</b><br>
<code>IPAddress <b>ip</b>(#,#,#,#)</code>: Static IP of device to request from router <br>
<code><IPAddress <b>gateway</b>(#,#,#,#)</code> <br>
<code>IPAddress <b>subnet</b>(#,#,#,#)</code> <br>
<code>const char* <b>ssid</b></code> <br>
<code>const char* <b>password</b></code> <br>
<code>bool <b>doorOpen</b></code>: Current position of the door <br>
<code>bool  <b>lightOn</b></code>: Current state of lights <br>
<code>String <b>webpage</b></code> HTML body <br>

<b>Pins Used:</b><table>
  <tr>
    <td>Indicator LED</td>
    <td>D1</td>
    <td>GIOP 5 / <code>LED_BUILTIN</code></td>
  </tr>
  <tr>
    <td>Garage door relay </td>
    <td>D2</td>
    <td>GIOP 4</td>
  </tr>
  <tr>
    <td>Light relay </td>
    <td>D3</td>
    <td>GIOP 0</td>
  </tr>
<table>

<b>Shortcuts & Android App:</b><br>
For IPhone create web shortcuts to the IP followed by the get string  to trigger <br>
each function from the home screen. <br>
For android I created a basic app using Tasker and exported it to APK to <br>
be able to install on multiple devices. <br>

<b>Web interface: </b><br>
Current static IP : <code>192.168.1.144</code> <br>
<code>http://InternalIP/DOOR?</code> & <code>http://InternalIP/LIGHT?</code>

<b>TODO: </b>
<ol>
  <li><Make webpage more user friendly with larger buttons and display current door & light status. </li>
  <li>Find better way to store web page instead of all in one variable</li>
  <li>Put in proximity sensor or limit switch to detect open or closed door</li>
  <li>Put photo resistor to detect day light and turn light on at night when door is opened</li>
</ol>

<b>Untested: </b> <br>
OTA updating abilities are currently untested but code is in there for possible<br>
future use. 
