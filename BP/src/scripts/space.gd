extends Node3D

@export var simulation_length: float = 1024.582953
@export var radial_velocity: Curve
@export var tangential_velocity: Curve
@export var pitch: Curve
@export var mass: Curve

var simulation_speed: float = 2.486851829
var current_time: float = 0.0
var time_normalized: float = 0.0
var altitude: float = 0.0

@onready var descend_path: PathFollow3D = $Trajectory/PathFollow3D
@onready var ui_overlay: CenterContainer = $UiOverlay
@onready var moon: MeshInstance3D = %Moon
@onready var vikram: MeshInstance3D = %Vikram


func _ready():
	current_time = 0.0
	
	
func _process(delta):
	current_time += delta * simulation_speed
	current_time = clamp(current_time, 0.0, simulation_length)
	time_normalized = current_time / simulation_length
	descend_path.progress_ratio = time_normalized
	# 0.25 scale of world
	altitude = ((moon.global_position).distance_to(vikram.global_position)
					- moon.scale.x) * 4
	
	vikram.rotation = Vector3(0.0, pitch.sample(time_normalized), -1 * PI / 2)
	
	ui_overlay.set_altitude(altitude)
	ui_overlay.set_time_normalized(time_normalized)
	ui_overlay.set_radial_velocity(radial_velocity.sample(time_normalized))
	ui_overlay.set_tangential_velocity(tangential_velocity.sample(time_normalized))
	ui_overlay.set_pitch_label(pitch.sample(time_normalized))
	ui_overlay.set_mass_label(mass.sample(time_normalized))
	ui_overlay.set_time_label(current_time)


func set_simulation_speed(new_speed):
	simulation_speed = new_speed


func set_simulation_time(new_time):
	current_time = new_time * simulation_length
	time_normalized = new_time	
