const char addtable_html[] PROGMEM = R"rawliteral(



<!DOCTYPE HTML><html>
<head>
<title>Add Shedule</title>
<meta name="viewport" content="width=device-width, initial-scale=1", charset="utf-8">

<style>
  html {font-family: Arial; display: inline-block; text-align: left;}
  h2 {font-size: 3.0rem; text-align: center}
  p {margin-left: 40px;}
  body {max-width: 600px; margin:0px auto; padding-bottom: 25px; margin-left: 15px}
 .button {
    position: relative; background-color: #404040;  border: none; font-size: 16px; margin: 4px 2px; color: white; padding: 20px;
    transition-duration: 0.4s;   text-decoration: none; overflow: hidden; cursor: pointer; display: inline-block;  border-radius: 20px;
      vertical-align: top;  }
 .button1:after { content: ""; background: #f1f1f1; display: block; position: absolute;  padding-top: 300;
    margin-left: -20px !important; margin-top: -120%; opacity: 0; transition: all 0.8s
 }
 .button1:active:after {  padding: 0; margin: 0; opacity: 1; transition: 0s}
 .button1:hover { background-color: #595959;  color: white; }
 .label { display: inline-block;
    font-size: 16px; 
  margin-top :10px;
  margin-right :10px;
  margin-left :-15px;
   vertical-align: center; 
   padding: 10px 10px 8px 
  }
  
  .weekDays-selector input {
  display: none!important;
}

.weekDays-selector input[type=checkbox] + label {
  display: inline-block;
  border-radius: 6px;
  background: #dddddd;
  height: 40px;
  width: 30px;
  margin-right: 3px;
  line-height: 40px;
  text-align: center;
  cursor: pointer;
}

.weekDays-selector input[type=checkbox]:checked + label {
  background: #2AD705;
  color: #ffffff;
}

</style>
</head>
<body>
  <h2>Smart Home</h2>
  
  <form onchange="selectTime(this)">
  <label for="appt"><span style="color:blue;font-weight:bold;font-size: 16px">Select time:&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;</span></label>
  %TIMEINPUT%
  </form>
  <p></p>
  <div class="weekDays-selector">
  <label><span style="color:blue;font-weight:bold;font-size: 16px">Select Days :&nbsp;&nbsp;&nbsp;</span></label>
  %WEEKDAYS%
  </div>
     
<p></p> 
<div >
  <form>
  <label for="devices" class="label"><span style="color:blue;font-weight:bold;font-size: 16px">&nbsp;Select device:</span></label>
  <select name="devices" id="devices" class="label">
    <option id="2" value="2">1</option>
    <option id="3" value="3">2</option>
    <option id="4" value="4">3</option>
    <option id="5" value="5">4</option>
    <option id="6" value="6">5</option>
    <option id="7" value="7">6</option>
    <option id="8" value="8">7</option>
    <option id="9" value="9">8</option>
    <option id="10" value="10">9</option>
    <option id="11" value="11">10</option>
    <option id="12" value="12">11</option>
    <option id="13" value="13">12</option>
  </select>
  <button type="button" class="button button1" onclick= "selectDevice(this)">+</button>
  <button type="button" class="button button1" onclick= "delDevice(this)">-</button>
  </form>
  </div>
  
  <p></p>
  <div id="extwaiokist" style="display:none" v="obocp" q="474c8fd6" c="373.0" i="25" u="0.954" s="10142115" d="1" w="false" e="" m="BMe=" vn="3gtr1">
    <div id="extwaigglbit" style="display:none" v="obocp" q="474c8fd6" c="373.0" i="25" u="0.954" s="10142115" d="1" w="false" e="" m="BMe=">
    </div>
    </div>
  <div>
  <label><span style="color:blue;font-weight:bold;font-size: 18px">state :&nbsp;&nbsp;&nbsp;</span></label>
  <input type="radio" id="on" name = "state" checked="">
  <label for="huey">ON &nbsp;&nbsp;</label> 
  <input type="radio" id="off" name = "state">
  <label for="dewey">OFF</label>
 </div>

<ul id="json" style="font-size: 16px;" >
</ul>


  <p></p>
  <a href="/Schedule" class="button button1" onclick="emitTable(this)">save / add</a>
  <a href="/Schedule" class="button button1">&nbsp;&nbsp;&nbsp;cancel&nbsp;&nbsp;&nbsp;</a>

<script>

%JSONARRAY%

function arrayRemove(arr, value) { 
        return arr.filter(function(ele){ 
            return ele != value; 
        });
    }

function drawJSON(json)
{
    let list = "";
    list += "<br><li style=\"font-size:16px\"><span style=\"color:red;font-weight:bold\">Devices : </span></li>";
    for (let x in json.devices){
      list += "<br><dd style=\"font-size:16px\">-"+ document.getElementById(x).innerHTML +" : ";
      if (json.devices[x] == 1 ){list += "on";}
      else {list += "off" ;}  
      list += "</dd>";
    }
    return list;
}

document.getElementById("json").innerHTML = drawJSON(data);

function selectTime(element)
  {
    data.time =  document.getElementById("appt").value;
    var xhttp = new XMLHttpRequest();
    xhttp.onreadystatechange = function()
     {
        if (this.readyState == 4 && this.status == 200)
        {
           if( this.responseText == 1){
            alert(JSON.stringify(data.time)+" : this time exist");
           }
        }
    }
   xhttp.open("GET", "/red?sure="+JSON.stringify(data.time), true);
   xhttp.send();   
  }
  
function selectDay(element)
  { 
    var dayVal = element.id;
    if (document.getElementById(element.id).checked){
      if (!(data.days.includes(dayVal))){
        data.days.push(dayVal);}
        return;
    }   
    data.days = arrayRemove(data.days, dayVal);
  }
  
function selectDevice(element)
  {
    var deviceVal =  document.getElementById("devices").value;
    var stateVal = 0;
    if (document.getElementById("on").checked){
      stateVal = 1;
    }
    data.devices[deviceVal] = stateVal;
    document.getElementById("json").innerHTML = drawJSON(data);
  }  
  
function delDevice(element)
  {
    var deviceVal =  document.getElementById("devices").value;
    delete data.devices[deviceVal];
    document.getElementById("json").innerHTML = drawJSON(data);
  }  
  
function emitTable(element){
   var xhr = new XMLHttpRequest();
   xhr.open("GET", "/red?save="+JSON.stringify(data), true);
   xhr.send();
 }
</script>

</body></html>



)rawliteral";
