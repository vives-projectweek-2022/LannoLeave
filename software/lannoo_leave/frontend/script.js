document.getElementById('mode_on').onclick = selector;
document.getElementById('mode_off').onclick = selector;

document.getElementById('effect_none').onclick = selector;
document.getElementById('effect_1').onclick = selector;
document.getElementById('effect_2').onclick = selector;
document.getElementById('effect_3').onclick = selector;

document.getElementById('submit_color').onclick = selector;

function selector(selected) {
  alert(this.id)
  
}