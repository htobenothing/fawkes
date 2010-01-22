BEGIN TRANSACTION;
CREATE TABLE config (
  path      TEXT NOT NULL,
  type      TEXT NOT NULL,
  value     NOT NULL,
  comment   TEXT,
  PRIMARY KEY (path)
);
INSERT INTO "config" VALUES('/fawkes/mainapp/blackboard_size','unsigned int','2097152','Size of BlackBoard memory segment; bytes');
INSERT INTO "config" VALUES('/fawkes/mainapp/desired_loop_time','unsigned int',30000,'Desired loop time of main thread, 0 to disable; microseconds');
INSERT INTO "config" VALUES('/fawkes/mainapp/max_thread_time','unsigned int',30000,'Maximum time a thread may run per loop, 0 to disable; microseconds');
INSERT INTO "config" VALUES('/fawkes/mainapp/net/tcp_port','unsigned int',1910,'Port for Fawkes Network Protocol service; TCP port');
INSERT INTO "config" VALUES('/fawkes/mainapp/net/service_name','string','Fawkes on %h','Name for Fawkes service, announced via Avahi, %h is replaced by short hostname');
INSERT INTO "config" VALUES('/fawkes/bbsync/check_interval','unsigned int',5000,'Interval between checking for remote BB aliveness; ms');
INSERT INTO "config" VALUES('/fawkes/bbsync/peers/localhost/host','string','localhost','Host name of remote peer; hostname');
INSERT INTO "config" VALUES('/fawkes/bbsync/peers/localhost/port','unsigned int',1911,'Port of remote peer; TCP port');
INSERT INTO "config" VALUES('/fawkes/bbsync/peers/localhost/active','bool','1','Is peer active? If omitted 1 is assumed.');
INSERT INTO "config" VALUES('/fawkes/bbsync/peers/localhost/check_interval','unsigned int',1000,'Interval in which to check for aliveness, if omitted defaults to 5 seconds; ms');
INSERT INTO "config" VALUES('/fawkes/bbsync/peers/localhost/reading/laser','string','Laser360Interface::Laser','Interface to synchronize, reading instance on remote, mapped to remote instance locally');
INSERT INTO "config" VALUES('/fawkes/bbsync/peers/localhost/reading/speechsynth','string','SpeechSynthInterface::Flite',NULL);
INSERT INTO "config" VALUES('/firevision/fountain/tcp_port','unsigned int',2208,'Fountain TCP Port; TCP port');
INSERT INTO "config" VALUES('/worldinfo/multicast_addr','string','224.16.0.1','Multicast address to send world info messages to.');
INSERT INTO "config" VALUES('/worldinfo/udp_port','unsigned int','2806','UDP port to listen for and send world info messages to; UDP port');
INSERT INTO "config" VALUES('/worldinfo/encryption_key','string','AllemaniACsX0rz','Default encryption key for world info.');
INSERT INTO "config" VALUES('/worldinfo/encryption_iv','string','DoesAnyOneCare','Default encryption initialization vector for world info.');
INSERT INTO "config" VALUES('/worldinfo/enable_fatmsg','bool','0','Send legacy fat message?');
INSERT INTO "config" VALUES('/worldinfo/sleep_time_msec','unsigned int','10','Time to sleep between two send/receive operations; miliseconds');
INSERT INTO "config" VALUES('/worldinfo/max_msgs_per_recv','unsigned int','20','Maximum number of messages to process per receive');
INSERT INTO "config" VALUES('/worldinfo/flush_time_sec','unsigned int','5','Time of inactivity after which the world info message sequence number is resetted; sec');
INSERT INTO "config" VALUES('/worldinfo/multicast_loopback','bool','1','Enable loopback for multicast messages?');
INSERT INTO "config" VALUES('/webview/port','unsigned int','10117','TCP port for Webview HTTP requests; TCP port');
INSERT INTO "config" VALUES('/ballposlog/log_level','unsigned int','0','Log level for ballposlog example plugin; sum of any of debug=0, info=1, warn=2, error=4, none=8');
INSERT INTO "config" VALUES('/skiller/skillspace','string','test','Skill space');
INSERT INTO "config" VALUES('/skiller/watch_files','bool',1,'Watch lua files for modification and automatically reload Lua if files have been changed; true to enable');
INSERT INTO "config" VALUES('/skiller/interfaces/test/reading/navigator','string','NavigatorInterface::Navigator',NULL);
INSERT INTO "config" VALUES('/skiller/interfaces/test/reading/wm_pose','string','ObjectPositionInterface::WM Pose',NULL);
INSERT INTO "config" VALUES('/skiller/interfaces/test/reading/speechsynth','string','SpeechSynthInterface::Festival',NULL);
INSERT INTO "config" VALUES('/skiller/interfaces/test/reading/katanaarm','string','KatanaInterface::Katana',NULL);
INSERT INTO "config" VALUES('/skiller/interfaces/test/reading/ptu_RX28','string','PanTiltInterface::PanTilt RX28',NULL);
INSERT INTO "config" VALUES('/skiller/interfaces/test/reading/ptu_EviD100P','string','PanTiltInterface::PanTilt EviD100P',NULL);
INSERT INTO "config" VALUES('/worldmodel/confspace','string','trunk',NULL);
INSERT INTO "config" VALUES('/worldmodel/wi_send/trunk/enable_send','bool','0','Enable sending of world info messages?');
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/pose/type','string','ObjectPositionInterface',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/pose/from_id','string','Pose',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/pose/to_id','string','WM Pose',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/pose/method','string','copy',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/obstacles/type','string','ObjectPositionInterface',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/obstacles/from_id','string','*Obstacle*',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/obstacles/to_id','string','WM Obstacle %u',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/obstacles/method','string','copy',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/ball/type','string','ObjectPositionInterface',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/ball/from_id','string','*Ball*',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/ball/to_id','string','WM Ball',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/ball/method','string','average',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/gamestate/type','string','GameStateInterface',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/gamestate/from_id','string','WI GameState',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/gamestate/to_id','string','WM GameState',NULL);
INSERT INTO "config" VALUES('/worldmodel/interfaces/trunk/gamestate/method','string','copy',NULL);
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/type','string','RX28','Type of PTU; one of RX28, EviD100P and DirectedPerceptionASCII');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/active','bool','1','Is this PTU active?');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/device','string','/dev/ttyUSB0','Device file for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/read_timeout_ms','unsigned int',30,'Read timeout for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/discover_timeout_ms','unsigned int',30,'Discover timeout for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/pan_servo_id','unsigned int',1,'Pan servo ID for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/tilt_servo_id','unsigned int',2,'Tilt servo ID for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/pan_zero_offset','int',-82,'Pan zero offset for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/tilt_zero_offset','int',0,'Tilt zero offset for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/turn_off','bool','1','Turn off RX28 PTU when unloading the plugin?');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/goto_zero_start','bool','1','Move RX28 PTU to (0,0) on start?');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/cw_compl_margin','unsigned int',0,'Clockwise compliance margin for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/ccw_compl_margin','unsigned int',0,'Counter-clockwise compliance margin for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/cw_compl_slope','unsigned int',96,'Clockwise compliance slope for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/ccw_compl_slope','unsigned int',96,'Counter-clockwise compliance slope for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/pan_min','float',-2.2,'Minimum pan for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/pan_max','float',2.2,'Maximum pan for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/tilt_min','float',-1.6,'Minimum tilt for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/RX28/tilt_max','float',1.6,'Maximum tilt for RX28 PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/EviD100P/type','string','EviD100P','Type of PTU; one of RX28, EviD100P and DirectedPerceptionASCII');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/EviD100P/active','bool','0','Is this PTU active?');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/EviD100P/device','string','/dev/ttyUSB1','Device file for EviD100P PTU');
INSERT INTO "config" VALUES('/hardware/pantilt/ptus/EviD100P/read_timeout_ms','unsigned int',50,'Read timeout for EviD100P PTU');
INSERT INTO "config" VALUES('/hardware/joystick/device_file','string','/dev/js0','Joystick device file');
INSERT INTO "config" VALUES('/hardware/katana/device','string','/dev/ttyUSB0','Device file of serial port the arm is connected to; file path');
INSERT INTO "config" VALUES('/hardware/katana/kni_conffile','string','/etc/kni/hd300/katana6M180.cfg','KNI configuration file; file path');
INSERT INTO "config" VALUES('/hardware/katana/auto_calibrate','bool','1','Automatically calibrate on startup?');
INSERT INTO "config" VALUES('/hardware/katana/default_max_speed','unsigned int',100,'Default maximum robot arm velocity, can be overridden via BB; [0..255]');
INSERT INTO "config" VALUES('/hardware/katana/read_timeout_msec','unsigned int',100,'Timeout for read operations; ms');
INSERT INTO "config" VALUES('/hardware/katana/write_timeout_msec','unsigned int',0,'Timeout for write operations; ms');
INSERT INTO "config" VALUES('/hardware/katana/gripper_pollint_msec','unsigned int',50,'Interval between gripper final checks; ms');
INSERT INTO "config" VALUES('/hardware/katana/goto_pollint_msec','unsigned int',50,'Interval between goto final checks; ms');
INSERT INTO "config" VALUES('/hardware/katana/park_x','float',66.863892,'X position for parking');
INSERT INTO "config" VALUES('/hardware/katana/park_y','float',16.27648,'Y position for parking');
INSERT INTO "config" VALUES('/hardware/katana/park_z','float',442.759186,'Z position for parking');
INSERT INTO "config" VALUES('/hardware/katana/park_phi','float',1.809579,'Phi position for parking');
INSERT INTO "config" VALUES('/hardware/katana/park_theta','float',0.357964,'Theta position for parking');
INSERT INTO "config" VALUES('/hardware/katana/park_psi','float',-0.271071,'Psi position for parking');
INSERT INTO "config" VALUES('/hardware/laser/interface_type','string','usb','Interface type, currently only usb');
INSERT INTO "config" VALUES('/hardware/laser/use_default','bool',0,'Use default settings from flash?');
INSERT INTO "config" VALUES('/hardware/laser/set_default','bool',0,'Store default settings in flash?');
INSERT INTO "config" VALUES('/hardware/laser/canonical_resolution','string','high','Canonical resolution, if set overrides rotation_freq and angle_step; one of low (1 deg, 20 Hz) or high (0.5 deg, 15 Hz)');
INSERT INTO "config" VALUES('/hardware/laser/rotation_freq','unsigned int',20,'Maximum rotation frequency; Hz');
INSERT INTO "config" VALUES('/hardware/laser/angle_step','unsigned int',16,'Angle step; 1/16th degree');
INSERT INTO "config" VALUES('/hardware/laser/max_pulse_freq','unsigned int',10800,'Max pulse frequency; Hz');
INSERT INTO "config" VALUES('/hardware/laser/profile_format','unsigned int',256,'Profile format, 0x0100 only distances, 0x0400 only echoes, 0x0500 both');
INSERT INTO "config" VALUES('/hardware/laser/can_id','unsigned int',683,'CAN ID of laser');
INSERT INTO "config" VALUES('/hardware/laser/can_id_resp','unsigned int',1808,'CAN ID response');
INSERT INTO "config" VALUES('/hardware/laser/sensor_id','unsigned int',16,'Sensor ID in laser');
INSERT INTO "config" VALUES('/hardware/laser/sensor_id_resp','unsigned int',3,'Sensor ID response');
INSERT INTO "config" VALUES('/hardware/laser/btr0btr1','unsigned int',20,'Baud rate key, 0x14 for 1 MBit/s');
INSERT INTO "config" VALUES('/hardware/laser/port','unsigned int',0,'Port, 0 for default');
INSERT INTO "config" VALUES('/hardware/laser/irq','unsigned int',0,'IRQ, 0 for default');
INSERT INTO "config" VALUES('/hardware/laser/num_init_tries','unsigned int',5,'Number of times to try initialization');
INSERT INTO "config" VALUES('/hardware/laser/mount_rotation','float',180,'Rotation of the laser towards front; degree');
INSERT INTO "config" VALUES('/hardware/laser/use_dead_spots_filter','bool','1','If true, the fflaser_calibrate tool must be used to gain information about dead spots, which are then extracted with a filter');
INSERT INTO "config" VALUES('/plugins/festival/voice','string','nitech_us_bdl_arctic_hts','Voice to use for Festival speech synth');
INSERT INTO "config" VALUES('/fawkes/bblogger/scenario','string','qatest','Current logging scenario');
INSERT INTO "config" VALUES('/fawkes/bblogger/qatest/buffering','bool',1,'Enable buffering for this scenario?');
INSERT INTO "config" VALUES('/fawkes/bblogger/qatest/flushing','bool',0,'Flush file stream after each chunk? Can severely influence performance, but when enabled allows real-time log watching.');
INSERT INTO "config" VALUES('/fawkes/bblogger/qatest/interfaces/test','string','TestInterface::BBLoggerTest',NULL);
COMMIT;
