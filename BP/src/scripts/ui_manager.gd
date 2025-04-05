extends CenterContainer

var space: Node3D
var simulation_speed: float = 2.0
var simulation_direction: float = 0.0
var time_normalized: float = 0.0
var editable: bool = true
var speed_up: bool = false

@onready var time_slider = %time_slider
@onready var altitude = %altitude_data
@onready var radial_velocity = %radial_velocity_data
@onready var tangential_velocity = %tangential_velocity_data
@onready var pitch = %pitch_data
@onready var mass = %mass_data
@onready var time = %time_data
@onready var speed_options = %OptionButton

var time_direction_backup: float = 0.0 # restore simulation speed after slider change

func _ready():
	space = get_parent()
	speed_options.get_popup().transparent_bg = true


func _process(_delta):
	if editable: # simulation's running and moving slider position
		time_slider.value = time_normalized
	else: # slider moves simulation time
		time_normalized = time_slider.value
		space.set_simulation_time(time_normalized)
		

func set_time_normalized(new_time: float):
	time_normalized = new_time


func _on_quit_button_button_down():
	get_tree().quit()


func _on_reload_button_button_down():
	get_tree().reload_current_scene()


func _on_option_button_item_selected(index: int): # Drop-down menu
	var new_speed = float(%OptionButton.get_item_text(index))
	if speed_up:
		space.set_simulation_speed((simulation_direction / simulation_speed) * new_speed)
	simulation_speed = new_speed


func _on_start_rev_fstbtn_button_down():
	speed_up = true
	simulation_direction = -1.0  * simulation_speed
	space.set_simulation_speed(simulation_direction)


func _on_start_rev_btn_button_down():
	speed_up = false
	simulation_direction = -1.0
	space.set_simulation_speed(simulation_direction)


func _on_pause_btn_button_down():
	speed_up = false
	simulation_direction = 0.0
	space.set_simulation_speed(0.0)


func _on_start_btn_button_down():
	speed_up = false
	simulation_direction = 1.0
	space.set_simulation_speed(simulation_direction)


func _on_start_fstbtn_button_down():
	speed_up = true
	simulation_direction = 1.0 * simulation_speed
	space.set_simulation_speed(simulation_direction)


func _on_time_slider_drag_started():
	editable = false
	time_direction_backup = simulation_direction
	simulation_direction = 0.0
	space.set_simulation_speed(simulation_direction)


func _on_time_slider_drag_ended(_value_changed):
	editable = true
	simulation_direction = time_direction_backup
	space.set_simulation_speed(time_direction_backup)


func set_altitude(new_altitude: float):
	altitude.text = "%.2f" % (new_altitude+134.97) + " m"


func set_radial_velocity(new_r_velocity: float):
	radial_velocity.text = "%.2f" % new_r_velocity + " m/s"


func set_tangential_velocity(new_t_velocity: float):
	tangential_velocity.text = "%.2f" % (new_t_velocity*10) + " m/s"
	

func set_pitch_label(new_pitch: float):
	if new_pitch < 0:
		new_pitch = 0.00
	pitch.text = "%.2f" % rad_to_deg(-new_pitch) + " deg"


func set_mass_label(new_mass: float):
	mass.text = "%.2f" % (new_mass*10) + " kg"


func set_time_label(new_time: float):
	time.text = "%.2f" % new_time + " s"
