var backend = "http://172.16.240.30:3000"
var units = 0; // MOET VIA GET REQUEST (of iets anders) MEEGEDEELD WORDEN 

$("#RGB").on('propertychange input', function(){
    var JsonData = JSON.stringify({"mode": "color", "unit": $('#select_unit').val(), "leaf": $('#select_leaf').val() , "led": $('#select_led').val() ,"color": $(this).val()});
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
let dropdownunit = $('#select_unit');
let dropdownleaf = $('#select_leaf');
let dropdownled = $('#select_led');

dropdownunit.empty();
dropdownleaf.empty();
dropdownled.empty();

dropdownunit.append('<option selected="true">all</option>');
dropdownunit.prop('selectedIndex', 0)
dropdownleaf.append('<option selected="true">all</option>');
dropdownleaf.prop('selectedIndex', 0)
dropdownled.append('<option selected="true">all</option>');
dropdownled.prop('selectedIndex', 0)

 // Count units
for(let i = 0; i < units; i++) {
    dropdownunit.append('<option>' + i + '</option>')
}

// leaf list
for(let i = 0; i < 8; i++) {
    dropdownleaf.append('<option>' + i + '</option>')
}

// Led list
for(let i = 0; i < 2; i++) {
    dropdownled.append('<option>' + i + '</option>')
}

