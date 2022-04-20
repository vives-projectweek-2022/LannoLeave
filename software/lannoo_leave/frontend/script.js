var backend = "http://172.16.240.30:3000"

$("#RGB").on('propertychange input', function(){
    var JsonData = JSON.stringify({"color": $(this).val()});
    post(JsonData, ""); 
  });

$(".select_mode").click(function() {
    //   JSON format: effect: 1; mode: on; etc
    var JsonData = JSON.stringify({"mode": this.id});
    post(JsonData, ""); 
});

$(".select_effect").click(function() {
    //   JSON format: effect: 1; mode: on; etc
    var JsonData = JSON.stringify({"effect": this.id});
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

dropdownleaf.append('<option selected="true" disabled>Select leaf</option>');
dropdownleaf.prop('selectedIndex', 0)
dropdownled.append('<option selected="true" disabled>Select led</option>');
dropdownled.prop('selectedIndex', 0)

var sample = '{"leaf1": "0","leaf2": "1","leaf3": "2","leaf4": "3"}';

array = $.parseJSON(sample);

$.each(array, function(key, value) {
    dropdownleaf.append('<option>' + value + '</option>')
})
