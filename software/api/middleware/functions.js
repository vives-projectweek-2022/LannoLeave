export function scaleToMatrix(coord_led_array) {
  const N = 12;
  coord_led_array = JSON.parse(coord_led_array);

  let smallest_x = null;
  let smallest_y = null;

  for (var pair of coord_led_array) {
    let x = pair[0].x;
    let y = pair[0].y;

    if (smallest_x == null || x < smallest_x) smallest_x = x;
    if (smallest_y == null || y < smallest_y) smallest_y = y;
  };

  for (var pair of coord_led_array) {
    pair[0].x += Math.abs(smallest_x);
    pair[0].y += Math.abs(smallest_y);
  }

  let biggest_x = null;
  let biggest_y = null;

  for (var pair of coord_led_array) {
    let x = pair[0].x;
    let y = pair[0].y;

    if (biggest_x == null || x > biggest_x) biggest_x = x;
    if (biggest_y == null || y > biggest_y) biggest_y = y;
  };

  for (var pair of coord_led_array) {
    pair[0].x = Math.floor(pair[0].x * (N / biggest_x));
    pair[0].y = Math.floor(pair[0].y * (N / biggest_y));
  }

  return coord_led_array;
}
