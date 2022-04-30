# Python image mapping

To map a image run python script as follow

```bash
python client.py --image /PATH/TO/IMAGE(.png, .gif, ...) --ip SERVER_IP --port PORT (optional default 3000)
```

## Precalculated images

In the directory save you will find subdirectory's named after the hash of the coordinates.

Inside those directory's the precalculated images are saved with the same name as the input image, the script will first check if a precalculated image exists if not it will calculate the image and then save in the correct directory.

