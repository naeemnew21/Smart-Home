

const char main_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<title>Smart Home</title>
<meta name="viewport" content="width=device-width, initial-scale=1", charset="utf-8">
<style>
  html {font-family: Arial; display: inline-block; text-align: center;}
  h2 {font-size: 3.0rem;}
  p {font-size: 3.0rem;}
  body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
 .button {
    position: relative; background-color: #404040;  border: none; font-size: 16px; margin: 4px 2px; color: white; padding: 20px;
    transition-duration: 0.4s;   text-decoration: none; overflow: hidden; cursor: pointer; display: inline-block;  border-radius: 20px;
 }
 .button1:after { content: ""; background: #f1f1f1; display: block; position: absolute;  padding-top: 300%; padding-left: 350%;
    margin-left: -20px !important; margin-top: -120%; opacity: 0; transition: all 0.8s
 }
 .button1:active:after {  padding: 0; margin: 0; opacity: 1; transition: 0s}
 .button1:hover { background-color: #595959;  color: white; }
  
</style>
</head>
<body>
  <h2>Smart Home</h2>
  <p></p>
  <p></p>
  <a href="room1" class="button button1">Room 1</button></a>
  <p></p>
  <a href="room2" class="button button1">Room 2</button></a>
  <p></p>
  <a href="Schedule" class="button button1">Shedule</button></a>

</body>
</html>
)rawliteral";
