import rospy
import tf
from sensor_msgs.msg import Image, CameraInfo
from geometry_msgs.msg import TransformStamped

class KinectPublisher:
    def __init__(self):
        self.subscriber_d = rospy.Subscriber('/camera/depth/image_raw', Image, self.depthCallback)
        self.subscriber_rgb = rospy.Subscriber('/camera/depth/rgb_image_raw', Image, self.rgbCallback)
        self.subscriber_info = rospy.Subscriber('/camera/depth/rgb_camera_info', CameraInfo, self.infoCallback)

        self.publisher_d = rospy.Publisher('/camera/registered/depth_image', Image, queue_size=20)
        self.publisher_rgb = rospy.Publisher('/camera/registered/rgb_image', Image, queue_size=20)
        self.publisher_info = rospy.Publisher('/camera/registered/rgb_camera_info', CameraInfo, queue_size=20)

    def depthCallback(self, msg):
        # Create a new msg which change the header frame_id to "camera_link"
        new_msg = msg
        # Publish the new msg
        self.publisher_d.publish(new_msg)
    
    def rgbCallback(self, msg):
        # Create a new msg which change the header frame_id to "camera_link"
        new_msg = msg
        # Publish the new msg
        self.publisher_rgb.publish(new_msg)
    
    def infoCallback(self, msg):
        # Create a new msg which change the header frame_id to "camera_link"
        new_msg = msg
        # Publish the new msg
        self.publisher_info.publish(new_msg)
    
    

if __name__ == '__main__':
    rospy.init_node('kinect_publisher')
    kp = KinectPublisher()
    rospy.spin()
