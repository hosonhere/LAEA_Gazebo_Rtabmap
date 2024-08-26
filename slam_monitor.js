var ros = new ROSLIB.Ros({
    url : 'ws://localhost:9090'
});

ros.on('connection', function() {
    console.log('Connected to websocket server.');
});

ros.on('error', function(error) {
    console.log('Error connecting to websocket server: ', error);
});

ros.on('close', function() {
    console.log('Connection to websocket server closed.');
});

// Create the main viewer.
var viewer = new ROS3D.Viewer({
    divID : 'viewer',
    // width : 800, // If you want to fill the entire window, use 'window.innerWidth'
    // height : 600, // If you want to fill the entire window, use 'window.innerHeight'
    width : window.innerWidth,
    height : window.innerHeight,
    antialias : true
});

// Setup a client to listen to TFs.
var tfClient = new ROSLIB.TFClient({
    ros : ros,
    angularThres : 0.0001,
    transThres : 0.0001,
    rate : 10.0,
    fixedFrame : '/world'
});

// Setup a client to listen to point cloud
var cloudClient = new ROS3D.PointCloud2({
    ros: ros,
    tfClient: tfClient,
    rootObject: viewer.scene,
    topic: '/rtp/pointcloud/map',
    material: { size: 0.3 },
    colorsrc: 'z',
    colormap: (z) => {
        let normalizedZ = (z + 0.1) / 2.4;
        return {
            r: normalizedZ,
            g: 0.0,
            b: 1.0 - normalizedZ,
            a: 1.0
        };
    },
    max_pts: 5000000
});

var marker = null;

// Draw a marker
function draw_marker(msg) {
    // Clean the marker if it exists
    if (marker !== null) {
        viewer.scene.remove(marker);
    }

    // Create a marker
    marker = new ROS3D.Marker({
        message: msg
    });

    // Add the marker to the scene
    viewer.scene.add(marker);
}

// Setup client to listen to a topic
var marker_topic_listener = new ROSLIB.Topic({
    ros : ros,
    name : '/rtp/position_vis',
    messageType : 'visualization_msgs/Marker'
});

// Only draw it in 60Hz
marker_topic_listener.subscribe(function(message) {
    // if (Date.now() - lastDrawTime > drawInterval) {
        // if (marker !== null) {
        //     viewer.scene.remove(marker);
        // }
        draw_marker(message);
        lastDrawTime = Date.now();
    // }
});
