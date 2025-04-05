extends Camera3D

@export_category("Camera Offsets")
@export var minimal_offset = Vector3(0.003, 0.003, 0.003)
@export var maximal_offset = Vector3(0.03, 0.03, 0.03)

@export_category("Camera Speed")
@export var zoom_speed = 0.5

var current_zoom = 0.0
@onready var player: MeshInstance3D = %Vikram
@onready var path: PathFollow3D = $".."
@onready var path_parent: Path3D = $"../.."
@onready var camera_stage: Node3D = $"../../.."
@onready var moon: MeshInstance3D = %Moon

var rot


func _input(event):
	if event is InputEventMouseMotion:
		rot = event.relative
	else:
		rot = Vector2(0.0, 0.0)
	
		
func _process(delta):
	camera_stage.position = player.global_position

	if Input.is_action_just_released("zoom_in"):
		change_zoom(false, delta)
	if Input.is_action_just_released("zoom_out"):
		change_zoom(true, delta)

	if Input.is_action_pressed("rotate_camera"):
		Input.mouse_mode = Input.MOUSE_MODE_CAPTURED
		path_parent.rotation += Vector3(rot.x, 0.0, rot.y) * delta * 0.05
	if Input.is_action_just_released("rotate_camera"):
		Input.mouse_mode = Input.MOUSE_MODE_VISIBLE
	

func change_zoom(out, delta):
	var change = 1 if out else -1	
	path.progress_ratio = clamp(path.progress_ratio + change * delta * zoom_speed, 0.0, 1.0)
