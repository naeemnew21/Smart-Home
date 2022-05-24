

const char Schedule_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
<title>Schedule</title>
<meta name="viewport" content="width=device-width, initial-scale=1", charset="utf-8">
	<style>
    html {font-family: Arial; display: inline-block; text-align: center;}
    h2 {font-size: 3.0rem;}
    body {max-width: 600px; margin:0px auto; padding-bottom: 10px;}
    .switch {position: relative; display: inline-block; width: 70px; height: 50px; margin-right: 5px}
      .switch input {display: none}
    .slider {position: absolute; top: 0; left: 0; right: 0; bottom: 0; background-color: #ccc; border-radius: 34px}
      .slider:before {position: absolute; content: ""; height: 40px; width: 40px; left: 6px; bottom: 5px; background-color: #fff; -webkit-transition: .4s; transition: .4s; border-radius: 68px}
      input:checked+.slider {background-color: #2196F3}
            input:checked+.slider:before {-webkit-transform: translateX(18px); -ms-transform: translateX(18px); transform: translateX(18px)}
    .button { border: none; border-radius: 68px ; width: 40px; height: 30px;color: white; text-align: center;text-decoration: none; display: inline-block; font-size: 18px; margin: 0px 2px; cursor: pointe ;vertical-align: top; padding: 12px 15px 8px }
      .button1 {background-color: #990000;text-indent: -5px} 
      .button1:hover { background-color: #cc0000; color: white }
        .button1:active { vertical-align: top; padding: 10px 15px 8px }
      .button2 {background-color: #404040; width: 80px; height: 30px; border-radius: 10px} 
      .button2:hover { background-color: #595959; color: white }
        .button2:active { vertical-align: top; padding: 10px 15px 8px }
    .label { display: inline-block; font-size: 16px;  margin-left: 0px; margin-right: 10px;  text-indent: -15px; vertical-align: top; padding: 10px 15px 8px  }
.collapsible {
  background-color: #f0f0f5;
  color: black;
  cursor: pointer;
  padding: 18px;
  width: 100%;
  border: none;
  text-align: left;
  outline: none;
  font-size: 15px;
  text-align: center;
  text-decoration: none;
  text-indent: -5px;
}
.collapsible2{
  background-color:#ccccff ;
  color: white;
}

.active, .collapsible:hover {
  background-color: #555;  
}

.collapsible:after {
  content: '\002B';
  color: white;
  font-weight: bold;
  float: right;
  margin-left: 5px;
}

.active:after {
  content: "\2212";
}

.content {
  padding: 0 18px;
  max-height: 0;
  overflow: hidden;
  transition: max-height 0.2s ease-out;
  background-color: #f1f1f1;
  font-size: 18px;
}
  </style>
</head>
<body>
  <h2>Schedule</h2>
  <p></p>
  %TABLE%
  <p></p>
 	 <a href="/addtable" class="button button2">Add Table</button></a>
   <a href="/" class="button button2">Main</button></a>


<script>
var coll = document.getElementsByClassName("collapsible");
var i;
for (i = 0; i < coll.length; i++) {
  coll[i].addEventListener("click", function() {
    this.classList.toggle("active");
    var content = this.nextElementSibling;
    if (content.style.maxHeight){
      content.style.maxHeight = null;
    } else {
      content.style.maxHeight = content.scrollHeight + "px";
    } 
  });
}

function toggleCheckbox(element)
  {
     var xhr = new XMLHttpRequest();
     if(element.checked)
     { xhr.open("GET", "/toggle?edit="+element.id+"&state=1", true);
     } else {
     xhr.open("GET", "/toggle?edit="+element.id+"&state=0", true);}
     xhr.send();
  }
  
</script>
</body>
</html>
)rawliteral";
