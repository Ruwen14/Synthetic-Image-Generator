"""
Name : main.py
Author  : Ruwen Kohm
Contact : ruwen.kohm@web.de
Time    : 23.08.2021 19:37
Desc:
"""

from PIL import Image, ImageEnhance, ImageDraw
from skimage import measure
import numpy as np

from shapely.geometry import Polygon

polya = Polygon([(0, 0), (0, 1), (1, 1), (1, 0)])
polyb = Polygon([(0.5, 0.5), (0.5, 0.8), (0.8, 0.8), (0.8, 0.5)])

polya.contains(polyb)

def close_contour(contour: np.ndarray) -> np.ndarray:
    if not np.array_equal(contour[0], contour[-1]):
        contour = np.vstack((contour, contour[0]))

    return contour

def binary_mask_to_polygon(binary_mask: Image, tolerance: int):
    """Converts a binary mask to COCO polygon representation

    Args:
        binary_mask: a 2D binary numpy array where '1's represent the object
        tolerance: Maximum distance from original points of polygon to approximated
            polygonal chain. If tolerance is 0, the original coordinate array is returned.

    """
    polygons = []

    # pad mask to close contours of shapes which start and end at an edge
    padded_binary_mask = np.pad(binary_mask, pad_width=1, mode='constant', constant_values=0)
    contours = measure.find_contours(padded_binary_mask, 0.5)
    # contours = np.subtract(contours, 1)
    for contour in contours:
        for i in range(len(contour)):
            row, col = contour[i]
            contour[i] = (row - 1, col - 1)

        contour = close_contour(contour)
        contour = measure.approximate_polygon(contour, tolerance)
        if len(contour) < 3:
            continue
        contour = np.flip(contour, axis=1)
        segmentation = contour.ravel().tolist()
        # after padding and subtracting 1 we may get -0.5 points in our segmentation
        segmentation = [0 if i < 0 else i for i in segmentation]
        polygons.append(segmentation)

    return polygons


if __name__ == '__main__':
    import cv2
    mask_tmp = Image.open('t_img_klein0.png')
    mask1 = mask_tmp.convert('L')
    binary_mask = np.array(np.greater(np.array(mask1), 200), dtype=np.uint8)

    cntrs = binary_mask_to_polygon(binary_mask, 1)

    draw = ImageDraw.Draw(mask_tmp)
    draw.polygon(cntrs[0], outline='blue')

    print(len(cntrs))
    mask_tmp.show()