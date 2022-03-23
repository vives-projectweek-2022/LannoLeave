var backend = "http://172.16.240.30:3000"


var color_input = document.getElementById("RGB");

color_input.addEventListener("input", function(){
    var color_value = color_input.value;
    
    var JsonData = JSON.stringify({"color": color_value});
    post(JsonData, ""); 
  }, false);


function selector(selected) {
//   alert(this.id)
var JsonData = JSON.stringify({"select": this.id});
post(JsonData, ""); 

}

function onload() {
    var selectors = document.getElementsByClassName("selector");
    for (let index = 0; index < selectors.length; index++) {
        selectors[index].onclick = selector;
    }
}

function post(data, url) {

    let xhr = new XMLHttpRequest();
    xhr.open("POST", backend + '/' + url);

    xhr.setRequestHeader("Accept", "application/json");
    xhr.setRequestHeader("Content-Type", "application/json");

    xhr.onreadystatechange = function () {
    if (xhr.readyState === 4) {
        console.log(xhr.responseText);
    }};

    xhr.send(data);
    
}