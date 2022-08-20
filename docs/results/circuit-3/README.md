# Results

## Circuit 3

<img src="./assets/cs_initial_image.png" width="50%" height="50%">

## Pre-processing

Convert image to grayscale

<img src="./assets/cs_preproc_grayscale.png" width="50%" height="50%">

Gaussian blur

<img src="./assets/cs_preproc_blur.png" width="50%" height="50%">

Adaptive threshold

<img src="./assets/cs_preproc_threshold.png" width="50%" height="50%">

Morphological dilation

<img src="./assets/cs_preproc_morph_dilation.png" width="50%" height="50%">

Thinning operation

<img src="./assets/cs_preproc_thinning.png" width="50%" height="50%">

## Segmentation

### Detection of connections

Morphological closing

<img src="./assets/cs_segment_connections_morph_close.png" width="50%" height="50%">

Morphological opening

<img src="./assets/cs_segment_connections_morph_open.png" width="50%" height="50%">

Intersection between the preprocessed image and the image without connections

<img src="./assets/cs_segment_connections_intersection.png" width="50%" height="50%">

Image with only the circuit connections

<img src="./assets/cs_segment_connections_only_conn.png" width="50%" height="50%">

Detected connections

<img src="./assets/cs_segment_connections_detected.png" width="50%" height="50%">

### Detection of components

Remove connections

<img src="./assets/cs_segment_components_remove_connections.png" width="50%" height="50%">

Morphological closing

<img src="./assets/cs_segment_components_morph_close.png" width="50%" height="50%">

Detected components

<img src="./assets/cs_segment_components_detected.png" width="50%" height="50%">

### Update of detected connections

Remove components

<img src="./assets/cs_segment_connections_remove_components.png" width="50%" height="50%">

Updated connections

<img src="./assets/cs_segment_connections_updated.png" width="50%" height="50%">

### Detection of nodes and update of connections

Detected nodes and updated connections

<img src="./assets/cs_segment_nodes_detected_connections_updated.png" width="50%" height="50%">

### Detection of connection points (ports) of components

Detected ports of components

<img src="./assets/cs_segment_components_ports_detected.png" width="50%" height="50%">

### Update list of detected components

Images with regions of interest (ROI) for components [here](./assets/)

### Detection of labels

Remove elements

<img src="./assets/cs_segment_labels_remove_elements.png" width="50%" height="50%">

Morphological closing

<img src="./assets/cs_segment_labels_morph_close.png" width="50%" height="50%">

Morphological opening

<img src="./assets/cs_segment_labels_morph_open.png" width="50%" height="50%">

Detected labels

<img src="./assets/cs_segment_labels_detected.png" width="50%" height="50%">

### Association of labels to the elements of the circuit

Boxes for connections and nodes

<img src="./assets/cs_segment_labels_associate_boxes_connections_nodes.png" width="50%" height="50%">

Images with ROI for labels with associated element ID [here](./assets/)

## Generation of images with regions of interest for components and labels

Images with ROI for components and labels [here](./assets/)

## Generation of segmentation map

[Map](./assets/segmentation_map.json)
