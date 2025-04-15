inlets = 1;
outlets = 1;

var MAX_SLIDERS = 64;
var CURRENT_SLIDERS = 1;
var MAX_COLUMNS = 8;
var SLIDER_X_MARGIN = 10; 
var SLIDER_Y_MARGIN = 3; 

// Global UI objects
var InputRoute = null;
var LatentSlider = new Array(MAX_SLIDERS);
var PathInlet = this.patcher.getnamed("input1");
var PathOutlet = this.patcher.getnamed("output1");
var PathSymbolInlet = this.patcher.getnamed("symbol_in")


// Global UI objects position
var InputRoutPos = [10, 80];
var OutputRoutPos = [10, 400];
var LatentSliderPos = [10, 120];
var LatentSliderSize = [65, 220];


var IN_PACK_NAME = "input_unpack"
var OUT_UNPACK_NAME = "output_pack"

// make presentation layout
function make_presentation_layout() {
	var x_grid = 0;
	var y_grid = 0;
	for (var i = 0; i < this.CURRENT_SLIDERS; i++) {
		post(i, "\n"); 
		var posX = x_grid * (LatentSliderSize[0] + SLIDER_X_MARGIN);
		var posY = y_grid * (LatentSliderSize[1] + SLIDER_Y_MARGIN);
		this.LatentSlider[i].setboxattr("presentation_rect", posX, posY, LatentSliderSize[0], LatentSliderSize[1]);
		x_grid += 1;
		if (x_grid >= MAX_COLUMNS) {
			x_grid = 0;
			y_grid += 1;
		}
	}
	//
}


// generate sliders
function sliders(n_sliders) {
	if (n_sliders > MAX_SLIDERS) error('cannot generate more than ' + String(MAX_SLIDERS) + ' sliders.')
	if (InputRoute != null) this.patcher.remove(InputRoute);
	// create input routing
	this.patcher.remove(this.patcher.getnamed(IN_PACK_NAME));
	InputRoute = this.patcher.newdefault(InputRoutPos[0], InputRoutPos[1], "mc.unpack~", n_sliders); 
	InputRoute.setattr("varname", IN_PACK_NAME);
	this.patcher.connect(PathInlet, 0, InputRoute, 0);
	// create output routing
	this.patcher.remove(this.patcher.getnamed(OUT_UNPACK_NAME));
	OutputRoute = this.patcher.newdefault(OutputRoutPos[0], OutputRoutPos[1], "mc.pack~", n_sliders); 
	OutputRoute.setattr("varname", OUT_UNPACK_NAME);
	this.patcher.connect(OutputRoute, 0, PathOutlet, 0);
	// delete existing sliders
	for (var i = 0; i < this.CURRENT_SLIDERS; i++) {
		this.patcher.remove(LatentSlider[i]);
	}
	// create sliders
	if (n_sliders < 1) {
		return
	}
	// create pak for symout
	var symoutOutlet = this.patcher.getnamed("symout");
	var symoutPos = symoutOutlet.getattr("patching_rect");
	var pakName = "pak"
	for (var i = 0; i < n_sliders; i++) {
		pakName = pakName + " f";
	}
	var PakObject = this.patcher.newdefault(symoutPos[0], symoutPos[1] - 20, pakName);
	this.patcher.connect(PakObject, 0, symoutOutlet, 0); 
	var x_grid = 0;
	var y_grid = 0;
	for (var i = 0; i < n_sliders; i++) {
		var posX = LatentSliderPos[0] + i * LatentSliderSize[0];
		var posY = LatentSliderPos[1];
		var currentSlider = this.patcher.newdefault(posX, posY, "bpatcher", "latent_slider");
		//post(i, posX, posY, LatentSliderSize[0], LatentSliderSize[1], "\n");
		//currentSlider.setboxattr("patching_rect", posX, posY, LatentSliderSize[0], LatentSliderSize[1]);
		
		currentSlider.setboxattr("varname", "slider"+String(i+1));
		this.patcher.connect(InputRoute, i, currentSlider, 0);
		
		// make layout
		var posX = x_grid * (LatentSliderSize[0] + SLIDER_X_MARGIN);
		var posY = y_grid * (LatentSliderSize[1] + SLIDER_Y_MARGIN);
		currentSlider.setboxattr("patching_rect", posX, posY, LatentSliderSize[0], LatentSliderSize[1]);
		currentSlider.setboxattr("presentation", 1); 
		x_grid += 1;
		if (x_grid >= this.MAX_COLUMNS) {
			x_grid = 0;
			y_grid += 1;
		}
		// connect to output
		this.patcher.connect(currentSlider, 0, OutputRoute, i);
		this.LatentSlider[i] = currentSlider;
		// connect to symout
		this.patcher.connect(currentSlider, 1, PakObject, i);
		// make receive obj
		var receiveObj = this.patcher.newdefault(posX, posY - 20, "receive", jsarguments[1]+"_"+String(i))
		this.patcher.connect(receiveObj, 0, currentSlider, 0)
	}
	CURRENT_SLIDERS = n_sliders;
	
	update_patching_rect(); 
}

function max_columns(n_columns) {
	if (n_columns == 0) {
		return;
	}
	if (n_columns > this.CURRENT_SLIDERS) error("Cannot set max columns " + String(n_columns) + " with " + String(this.CURRENT_SLIDERS) + " sliders\n");
	this.MAX_COLUMNS = n_columns;
	//make_presentation_layout();
	update_patching_rect(); 
}

function update_patching_rect() {
	if (this.patcher.box != null) {
		var slider_rect = this.patcher.getnamed("slider1").getboxattr("patching_rect");
		var patching_rect = this.patcher.box.getboxattr("patching_rect")
		var presentation_rect = this.patcher.box.getboxattr("presentation_rect") 
		var target_width = Math.min(this.CURRENT_SLIDERS, this.MAX_COLUMNS) * (slider_rect[2] + SLIDER_X_MARGIN);
		var target_height = Math.ceil(this.CURRENT_SLIDERS / this.MAX_COLUMNS) * (slider_rect[3] + SLIDER_Y_MARGIN);
		this.patcher.box.setboxattr("patching_rect", patching_rect[0], patching_rect[1], target_width, target_height);
		this.patcher.box.setboxattr("presentation_rect", presentation_rect[0], presentation_rect[1], target_width, target_height);
	}
}

function slider_check_size() {
	if (this.patcher.box != null) {
		var slider_rect = LatentSliderSize;
		var patching_rect = this.patcher.box.getboxattr("patching_rect");
		this.patcher.box.setboxattr("patching_rect", patching_rect[0], patching_rect[1], LatentSliderSize[0], LatentSliderSize[1])
		var presentation_rect = this.patcher.box.getboxattr("presentation_rect");
		this.patcher.box.setboxattr("presentation_rect", patching_rect[0], patching_rect[1], LatentSliderSize[0], LatentSliderSize[1])
	}
}

function clear() {
	for (var i = 0; i < this.CURRENT_SLIDERS; i++) {
		this.patcher.remove(LatentSlider[i]); 
	}
	this.patcher.remove(this.patcher.getnamed(IN_PACK_NAME));
	this.patcher.remove(this.patcher.getnamed(OUT_UNPACK_NAME));
}


function faders() {
	var args = arrayfromargs(arguments);
	var mess = args.shift();
	//post("n sliders", CURRENT_SLIDERS);
	for (var i = 0; i < CURRENT_SLIDERS; i++) {
		//post('setting slider', i, 'to', args[i], '\n'); 
		messnamed(jsarguments[1]+"_"+String(i), mess, args[i])
	}
}

function faders_all() {
	var args = arrayfromargs(arguments);
	var mess = args.shift();
	//post("n sliders", CURRENT_SLIDERS);
	for (var i = 0; i < CURRENT_SLIDERS; i++) {
		//post('setting slider', i, 'to', args[i], '\n'); 
		messnamed(jsarguments[1]+"_"+String(i), mess, args)
	}
}


function fader() {
	var args = arrayfromargs(arguments);
	var fader_idx = args.shift() - 1;
	var mess = args.shift();
	if (fader_idx > CURRENT_SLIDERS) {
		error('fader_idx '+String(fader_idx)+' too big')
	}
	//post("n sliders", CURRENT_SLIDERS);
	messnamed(jsarguments[1]+"_"+String(fader_idx), mess, args)
}

function dump_all() {
	post("dump all!!")
}