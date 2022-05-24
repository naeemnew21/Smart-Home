
const char room1_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<title>room  1</title>
<meta name="viewport" content="width=device-width, initial-scale=1", charset="utf-8">
	<style>
		html {font-family: Arial; display: inline-block; text-align: center;}
		h2 {font-size: 3.0rem;}
		p {font-size: 3.0rem;}
		body {max-width: 600px; margin:0px auto; padding-bottom: 25px;}
		.switch {position: relative; display: inline-block; width: 120px; height: 68px;}
			.switch input {display: none}
		.slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #F90000; border-radius: 34px}
			.slider:before {position: absolute; content: ""; height: 52px; width: 52px; left: 8px; bottom: 8px; background-color: #Ffffff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
			 input:checked+.slider {background-color: #00EB25}
 			input:checked+.slider:before {-webkit-transform: translateX(52px); -ms-transform: translateX(52px); transform: translateX(52px)}
    .button { border: none;  width: 100px; height: 50px; border-radius: 4010px ; background-color: #404040; color: white; text-align: center;text-decoration: none; display: inline-block; font-size: 28px; margin: 0px 2px; cursor: pointe ;vertical-align: top; padding: 12px 15px 8px }
      .button2:hover { background-color: #595959; color: white }
      .button2:active { vertical-align: top; padding: 10px 15px 8px }
    .label { display: inline-block; font-size: 16px;  margin-left: 0px; margin-right: 50px;  text-indent: -15px; vertical-align: top; padding: 20px 20px 8px  }
	</style>
</head>
<body>
  <h2>room  1</h2>
  <div>
  <label class="label"><span style="color:blue;font-weight:bold; font-size: 20px">Light 1</span></label>
  %BUTTONPLACEHOLDER2%
  <p></p>
  <label class="label"><span style="color:blue;font-weight:bold; font-size: 20px">Light 2</span></label>
  %BUTTONPLACEHOLDER3%
  <p></p>
  <label class="label"><span style="color:blue;font-weight:bold; font-size: 20px">Light 3</span></label>
  %BUTTONPLACEHOLDER4%
  <p></p>
  <label class="label"><span style="color:blue;font-weight:bold; font-size: 20px">Light 4</span></label>
  %BUTTONPLACEHOLDER5%
  <p></p>
  <label class="label"><span style="color:blue;font-weight:bold; font-size: 20px">Light 5</span></label>
  %BUTTONPLACEHOLDER6%
  <p></p>
  <label class="label"><span style="color:blue;font-weight:bold; font-size: 20px">Light 6</span></label>
  %BUTTONPLACEHOLDER7%
  <p></p>
  </div>
  <p></p>
  <a href="/" class="button button2">Main</button></a>

  <script>

	function toggleCheckbox(element)
  {
    var xhr = new XMLHttpRequest();
    if(element.checked)
    { xhr.open("GET", "/update?output="+element.id+"&state=1", true);
    } else {
      xhr.open("GET", "/update?output="+element.id+"&state=0", true);}
    xhr.send();
}
 
 
setInterval(function ( )
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function()
  {
    if (this.readyState == 4 && this.status == 200)
    {
      const obj = JSON.parse(this.responseText);
      document.getElementById("2").checked = Boolean(Number(obj.relay2));
      document.getElementById("3").checked = Boolean(Number(obj.relay3));
      document.getElementById("4").checked = Boolean(Number(obj.relay4));
      document.getElementById("5").checked = Boolean(Number(obj.relay5));
      document.getElementById("6").checked = Boolean(Number(obj.relay6));
      document.getElementById("7").checked = Boolean(Number(obj.relay7));
    }
  }
  xhttp.open("GET", "/refresh", true);
  xhttp.send();   
}, 1000 ) ;


  </script>
</body>
</html>
)rawliteral";
