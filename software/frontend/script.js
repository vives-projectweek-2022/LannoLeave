var backend = "http://172.16.240.30:3000"

$("#RGB").on('propertychange input', function(){
    var JsonData = JSON.stringify({"mode": "color", "leaf": $('#select_leaf').val() , "led": $('#select_led').val() ,"color": $(this).val()});
    post(JsonData, ""); 
  });

$(".select_mode").click(function() {
    //   JSON format: effect: 1; mode: on; etc
    var JsonData = JSON.stringify({"mode": this.id});
    post(JsonData, ""); 
});

$(".select_effect").click(function() {
    //   JSON format: effect: 1; mode: on; etc
    var JsonData = JSON.stringify({"mode": "effect", "effect": this.id});
    post(JsonData, ""); 
});

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

let dropdownleaf = $('#select_leaf');
let dropdownled = $('#select_led');

dropdownleaf.empty();
dropdownled.empty();

dropdownleaf.append('<option selected="true">all</option>');
dropdownleaf.prop('selectedIndex', 0)
dropdownled.append('<option selected="true">all</option>');
dropdownled.prop('selectedIndex', 0)

var leaves = 5;
 // Count leaves
    for(let i = 0; i < leaves; i++) {
        dropdownleaf.append('<option>' + i + '</option>')
    }

// Led list (pre-set as every leaf has 16 leds)
    for(let i = 0; i < 16; i++) {
        dropdownled.append('<option>' + i + '</option>')
    }

