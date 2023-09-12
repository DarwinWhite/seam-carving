# seam-carving

This project uses an algorithm to identify and delete the least used pixels in a photo in order to make the photo smaller. This is done by calculating the energy of each pixel, a process by which a pixel is compared to pixels near it and then identified as being high energy (steep color gradient) or low energy (shallow color gradient). The lowest energy pixels are then removed.
