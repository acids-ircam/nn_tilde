{
	"patcher" : 	{
		"fileversion" : 1,
		"appversion" : 		{
			"major" : 8,
			"minor" : 6,
			"revision" : 5,
			"architecture" : "x64",
			"modernui" : 1
		}
,
		"classnamespace" : "box",
		"rect" : [ 100.0, 100.0, 814.0, 598.0 ],
		"bglocked" : 0,
		"openinpresentation" : 0,
		"default_fontsize" : 12.0,
		"default_fontface" : 0,
		"default_fontname" : "Arial",
		"gridonopen" : 1,
		"gridsize" : [ 15.0, 15.0 ],
		"gridsnaponopen" : 1,
		"objectsnaponopen" : 1,
		"statusbarvisible" : 2,
		"toolbarvisible" : 1,
		"lefttoolbarpinned" : 0,
		"toptoolbarpinned" : 0,
		"righttoolbarpinned" : 0,
		"bottomtoolbarpinned" : 0,
		"toolbars_unpinned_last_save" : 0,
		"tallnewobj" : 0,
		"boxanimatetime" : 200,
		"enablehscroll" : 1,
		"enablevscroll" : 1,
		"devicewidth" : 0.0,
		"description" : "",
		"digest" : "",
		"tags" : "",
		"style" : "",
		"subpatcher_template" : "",
		"assistshowspatchername" : 0,
		"boxes" : [ 			{
				"box" : 				{
					"hidden" : 1,
					"id" : "obj-17",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 699.0, 103.0, 70.0, 22.0 ],
					"text" : "loadmess 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-16",
					"maxclass" : "number",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 372.0, 99.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-13",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 372.0, 128.0, 105.0, 22.0 ],
					"text" : "join i f @triggers 1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-11",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 148.0, 453.0, 190.0, 22.0 ],
					"text" : "0.4414 0.618 0.752156 0.469841"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-8",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 489.0, 386.0, 119.0, 22.0 ],
					"presentation_linecount" : 2,
					"text" : "faders_all range -5 5"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-7",
					"maxclass" : "newobj",
					"numinlets" : 1,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 489.0, 345.0, 60.0, 22.0 ],
					"text" : "loadmess"
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 0,
					"fontsize" : 12.0,
					"id" : "obj-6",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 360.0, 423.0, 268.0, 52.0 ],
					"presentation_linecount" : 2,
					"text" : "user faders_all \"param\" \"values\" to set all slider's parameters to a single value"
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"fontsize" : 12.0,
					"id" : "obj-3",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 512.0, 289.0, 268.0, 37.0 ],
					"presentation_linecount" : 2,
					"text" : "user faders \"fader_idx\" \"param\" \"values\" to set individual slider's parameters with a list"
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"fontsize" : 12.0,
					"id" : "obj-2",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 466.0, 156.0, 268.0, 37.0 ],
					"presentation_linecount" : 2,
					"text" : "user fader \"fader_idx\" \"param\" \"values\" to set individually each slider's parameters"
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-46",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 376.0, 348.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-45",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 376.0, 386.0, 97.0, 22.0 ],
					"text" : "faders_all set $1"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-44",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 376.0, 296.0, 128.0, 22.0 ],
					"text" : "faders set $1 $2 $3 $4"
				}

			}
, 			{
				"box" : 				{
					"contdata" : 1,
					"id" : "obj-42",
					"maxclass" : "multislider",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 376.0, 217.0, 77.0, 63.0 ],
					"size" : 4
				}

			}
, 			{
				"box" : 				{
					"format" : 6,
					"id" : "obj-39",
					"maxclass" : "flonum",
					"numinlets" : 1,
					"numoutlets" : 2,
					"outlettype" : [ "", "bang" ],
					"parameter_enable" : 0,
					"patching_rect" : [ 485.0, 99.0, 50.0, 22.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-37",
					"maxclass" : "message",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "" ],
					"patching_rect" : [ 372.0, 161.0, 89.0, 22.0 ],
					"text" : "fader $1 set $2"
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-12",
					"maxclass" : "scope~",
					"numinlets" : 2,
					"numoutlets" : 0,
					"patching_rect" : [ 38.0, 453.0, 96.0, 94.0 ]
				}

			}
, 			{
				"box" : 				{
					"id" : "obj-9",
					"maxclass" : "newobj",
					"numinlets" : 2,
					"numoutlets" : 1,
					"outlettype" : [ "multichannelsignal" ],
					"patching_rect" : [ 38.0, 161.0, 72.0, 22.0 ],
					"text" : "mc.cycle~ 4"
				}

			}
, 			{
				"box" : 				{
					"args" : [ 4 ],
					"bgmode" : 0,
					"border" : 0,
					"clickthrough" : 0,
					"enablehscroll" : 0,
					"enablevscroll" : 0,
					"id" : "obj-4",
					"lockeddragscroll" : 0,
					"lockedsize" : 0,
					"maxclass" : "bpatcher",
					"name" : "latent_remote.maxpat",
					"numinlets" : 2,
					"numoutlets" : 3,
					"offset" : [ 0.0, 0.0 ],
					"outlettype" : [ "multichannelsignal", "", "" ],
					"patching_rect" : [ 38.0, 201.0, 300.0, 223.0 ],
					"varname" : "latent_remote",
					"viewvisibility" : 1
				}

			}
, 			{
				"box" : 				{
					"bubble" : 1,
					"bubbleside" : 2,
					"fontsize" : 16.0,
					"id" : "obj-1",
					"linecount" : 2,
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 34.0, 98.0, 287.0, 61.0 ],
					"text" : "create a \"bpatcher latent_remote @args X\" object to create X sliders ↓"
				}

			}
, 			{
				"box" : 				{
					"fontsize" : 45.0,
					"id" : "obj-5",
					"maxclass" : "comment",
					"numinlets" : 1,
					"numoutlets" : 0,
					"patching_rect" : [ 34.0, 18.0, 645.0, 57.0 ],
					"text" : "latent_remote"
				}

			}
 ],
		"lines" : [ 			{
				"patchline" : 				{
					"destination" : [ "obj-37", 0 ],
					"source" : [ "obj-13", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 0 ],
					"source" : [ "obj-16", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-16", 0 ],
					"hidden" : 1,
					"source" : [ "obj-17", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 1 ],
					"source" : [ "obj-37", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-13", 1 ],
					"source" : [ "obj-39", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-11", 1 ],
					"source" : [ "obj-4", 1 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-12", 0 ],
					"source" : [ "obj-4", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-44", 0 ],
					"source" : [ "obj-42", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 1 ],
					"source" : [ "obj-44", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 1 ],
					"source" : [ "obj-45", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-45", 0 ],
					"source" : [ "obj-46", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-8", 0 ],
					"source" : [ "obj-7", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 1 ],
					"source" : [ "obj-8", 0 ]
				}

			}
, 			{
				"patchline" : 				{
					"destination" : [ "obj-4", 0 ],
					"source" : [ "obj-9", 0 ]
				}

			}
 ],
		"parameters" : 		{
			"obj-4::obj-12::obj-3::obj-117" : [ "range_max[8]", "live.numbox", 0 ],
			"obj-4::obj-12::obj-3::obj-118" : [ "range_min[8]", "live.numbox", 0 ],
			"obj-4::obj-12::obj-3::obj-119" : [ "freq[8]", "live.numbox", 0 ],
			"obj-4::obj-12::obj-3::obj-120" : [ "phase[8]", "live.numbox", 0 ],
			"obj-4::obj-12::obj-3::obj-121" : [ "amp[8]", "live.numbox", 0 ],
			"obj-4::obj-12::obj-3::obj-137" : [ "live.toggle[19]", "live.toggle[2]", 0 ],
			"obj-4::obj-12::obj-3::obj-235" : [ "free[8]", "live.numbox", 0 ],
			"obj-4::obj-12::obj-3::obj-58" : [ "input_scale[8]", "scale", 0 ],
			"obj-4::obj-12::obj-3::obj-90" : [ "scale[8]", "live.menu", 0 ],
			"obj-4::obj-12::obj-3::obj-91" : [ "mode[8]", "live.menu", 0 ],
			"obj-4::obj-12::obj-3::obj-96" : [ "live.toggle[20]", "live.toggle[2]", 0 ],
			"obj-4::obj-12::obj-3::obj-99" : [ "clip[8]", "live.toggle", 0 ],
			"obj-4::obj-19::obj-3::obj-117" : [ "range_max[9]", "live.numbox", 0 ],
			"obj-4::obj-19::obj-3::obj-118" : [ "range_min[1]", "live.numbox", 0 ],
			"obj-4::obj-19::obj-3::obj-119" : [ "freq[1]", "live.numbox", 0 ],
			"obj-4::obj-19::obj-3::obj-120" : [ "phase[9]", "live.numbox", 0 ],
			"obj-4::obj-19::obj-3::obj-121" : [ "amp[1]", "live.numbox", 0 ],
			"obj-4::obj-19::obj-3::obj-137" : [ "live.toggle[4]", "live.toggle[2]", 0 ],
			"obj-4::obj-19::obj-3::obj-235" : [ "free[9]", "live.numbox", 0 ],
			"obj-4::obj-19::obj-3::obj-58" : [ "input_scale[1]", "scale", 0 ],
			"obj-4::obj-19::obj-3::obj-90" : [ "scale[1]", "live.menu", 0 ],
			"obj-4::obj-19::obj-3::obj-91" : [ "mode[1]", "live.menu", 0 ],
			"obj-4::obj-19::obj-3::obj-96" : [ "live.toggle[5]", "live.toggle[2]", 0 ],
			"obj-4::obj-19::obj-3::obj-99" : [ "clip[9]", "live.toggle", 0 ],
			"obj-4::obj-23::obj-3::obj-117" : [ "range_max[1]", "live.numbox", 0 ],
			"obj-4::obj-23::obj-3::obj-118" : [ "range_min[2]", "live.numbox", 0 ],
			"obj-4::obj-23::obj-3::obj-119" : [ "freq[2]", "live.numbox", 0 ],
			"obj-4::obj-23::obj-3::obj-120" : [ "phase[1]", "live.numbox", 0 ],
			"obj-4::obj-23::obj-3::obj-121" : [ "amp[2]", "live.numbox", 0 ],
			"obj-4::obj-23::obj-3::obj-137" : [ "live.toggle[22]", "live.toggle[2]", 0 ],
			"obj-4::obj-23::obj-3::obj-235" : [ "free[1]", "live.numbox", 0 ],
			"obj-4::obj-23::obj-3::obj-58" : [ "input_scale[2]", "scale", 0 ],
			"obj-4::obj-23::obj-3::obj-90" : [ "scale[2]", "live.menu", 0 ],
			"obj-4::obj-23::obj-3::obj-91" : [ "mode[2]", "live.menu", 0 ],
			"obj-4::obj-23::obj-3::obj-96" : [ "live.toggle[21]", "live.toggle[2]", 0 ],
			"obj-4::obj-23::obj-3::obj-99" : [ "clip[1]", "live.toggle", 0 ],
			"obj-4::obj-27::obj-3::obj-117" : [ "range_max[2]", "live.numbox", 0 ],
			"obj-4::obj-27::obj-3::obj-118" : [ "range_min[9]", "live.numbox", 0 ],
			"obj-4::obj-27::obj-3::obj-119" : [ "freq[3]", "live.numbox", 0 ],
			"obj-4::obj-27::obj-3::obj-120" : [ "phase[10]", "live.numbox", 0 ],
			"obj-4::obj-27::obj-3::obj-121" : [ "amp[9]", "live.numbox", 0 ],
			"obj-4::obj-27::obj-3::obj-137" : [ "live.toggle[6]", "live.toggle[2]", 0 ],
			"obj-4::obj-27::obj-3::obj-235" : [ "free[2]", "live.numbox", 0 ],
			"obj-4::obj-27::obj-3::obj-58" : [ "input_scale[9]", "scale", 0 ],
			"obj-4::obj-27::obj-3::obj-90" : [ "scale[9]", "live.menu", 0 ],
			"obj-4::obj-27::obj-3::obj-91" : [ "mode[3]", "live.menu", 0 ],
			"obj-4::obj-27::obj-3::obj-96" : [ "live.toggle[7]", "live.toggle[2]", 0 ],
			"obj-4::obj-27::obj-3::obj-99" : [ "clip[2]", "live.toggle", 0 ],
			"parameterbanks" : 			{
				"0" : 				{
					"index" : 0,
					"name" : "",
					"parameters" : [ "-", "-", "-", "-", "-", "-", "-", "-" ]
				}

			}
,
			"parameter_overrides" : 			{
				"obj-4::obj-12::obj-3::obj-117" : 				{
					"parameter_longname" : "range_max[8]"
				}
,
				"obj-4::obj-12::obj-3::obj-118" : 				{
					"parameter_longname" : "range_min[8]"
				}
,
				"obj-4::obj-12::obj-3::obj-119" : 				{
					"parameter_longname" : "freq[8]"
				}
,
				"obj-4::obj-12::obj-3::obj-120" : 				{
					"parameter_longname" : "phase[8]"
				}
,
				"obj-4::obj-12::obj-3::obj-121" : 				{
					"parameter_longname" : "amp[8]"
				}
,
				"obj-4::obj-12::obj-3::obj-137" : 				{
					"parameter_longname" : "live.toggle[19]"
				}
,
				"obj-4::obj-12::obj-3::obj-235" : 				{
					"parameter_longname" : "free[8]"
				}
,
				"obj-4::obj-12::obj-3::obj-58" : 				{
					"parameter_longname" : "input_scale[8]"
				}
,
				"obj-4::obj-12::obj-3::obj-90" : 				{
					"parameter_longname" : "scale[8]"
				}
,
				"obj-4::obj-12::obj-3::obj-91" : 				{
					"parameter_longname" : "mode[8]"
				}
,
				"obj-4::obj-12::obj-3::obj-96" : 				{
					"parameter_longname" : "live.toggle[20]"
				}
,
				"obj-4::obj-12::obj-3::obj-99" : 				{
					"parameter_longname" : "clip[8]"
				}
,
				"obj-4::obj-19::obj-3::obj-117" : 				{
					"parameter_longname" : "range_max[9]"
				}
,
				"obj-4::obj-19::obj-3::obj-118" : 				{
					"parameter_longname" : "range_min[1]"
				}
,
				"obj-4::obj-19::obj-3::obj-119" : 				{
					"parameter_longname" : "freq[1]"
				}
,
				"obj-4::obj-19::obj-3::obj-120" : 				{
					"parameter_longname" : "phase[9]"
				}
,
				"obj-4::obj-19::obj-3::obj-121" : 				{
					"parameter_longname" : "amp[1]"
				}
,
				"obj-4::obj-19::obj-3::obj-137" : 				{
					"parameter_longname" : "live.toggle[4]"
				}
,
				"obj-4::obj-19::obj-3::obj-235" : 				{
					"parameter_longname" : "free[9]"
				}
,
				"obj-4::obj-19::obj-3::obj-58" : 				{
					"parameter_longname" : "input_scale[1]"
				}
,
				"obj-4::obj-19::obj-3::obj-90" : 				{
					"parameter_longname" : "scale[1]"
				}
,
				"obj-4::obj-19::obj-3::obj-91" : 				{
					"parameter_longname" : "mode[1]"
				}
,
				"obj-4::obj-19::obj-3::obj-96" : 				{
					"parameter_longname" : "live.toggle[5]"
				}
,
				"obj-4::obj-19::obj-3::obj-99" : 				{
					"parameter_longname" : "clip[9]"
				}
,
				"obj-4::obj-23::obj-3::obj-117" : 				{
					"parameter_longname" : "range_max[1]"
				}
,
				"obj-4::obj-23::obj-3::obj-118" : 				{
					"parameter_longname" : "range_min[2]"
				}
,
				"obj-4::obj-23::obj-3::obj-119" : 				{
					"parameter_longname" : "freq[2]"
				}
,
				"obj-4::obj-23::obj-3::obj-120" : 				{
					"parameter_longname" : "phase[1]"
				}
,
				"obj-4::obj-23::obj-3::obj-121" : 				{
					"parameter_longname" : "amp[2]"
				}
,
				"obj-4::obj-23::obj-3::obj-137" : 				{
					"parameter_longname" : "live.toggle[22]"
				}
,
				"obj-4::obj-23::obj-3::obj-235" : 				{
					"parameter_longname" : "free[1]"
				}
,
				"obj-4::obj-23::obj-3::obj-58" : 				{
					"parameter_longname" : "input_scale[2]"
				}
,
				"obj-4::obj-23::obj-3::obj-90" : 				{
					"parameter_longname" : "scale[2]"
				}
,
				"obj-4::obj-23::obj-3::obj-91" : 				{
					"parameter_longname" : "mode[2]"
				}
,
				"obj-4::obj-23::obj-3::obj-96" : 				{
					"parameter_longname" : "live.toggle[21]"
				}
,
				"obj-4::obj-23::obj-3::obj-99" : 				{
					"parameter_longname" : "clip[1]"
				}
,
				"obj-4::obj-27::obj-3::obj-117" : 				{
					"parameter_longname" : "range_max[2]"
				}
,
				"obj-4::obj-27::obj-3::obj-118" : 				{
					"parameter_longname" : "range_min[9]"
				}
,
				"obj-4::obj-27::obj-3::obj-119" : 				{
					"parameter_longname" : "freq[3]"
				}
,
				"obj-4::obj-27::obj-3::obj-120" : 				{
					"parameter_longname" : "phase[10]"
				}
,
				"obj-4::obj-27::obj-3::obj-121" : 				{
					"parameter_longname" : "amp[9]"
				}
,
				"obj-4::obj-27::obj-3::obj-137" : 				{
					"parameter_longname" : "live.toggle[6]"
				}
,
				"obj-4::obj-27::obj-3::obj-235" : 				{
					"parameter_longname" : "free[2]"
				}
,
				"obj-4::obj-27::obj-3::obj-58" : 				{
					"parameter_longname" : "input_scale[9]"
				}
,
				"obj-4::obj-27::obj-3::obj-90" : 				{
					"parameter_longname" : "scale[9]"
				}
,
				"obj-4::obj-27::obj-3::obj-91" : 				{
					"parameter_longname" : "mode[3]"
				}
,
				"obj-4::obj-27::obj-3::obj-96" : 				{
					"parameter_longname" : "live.toggle[7]"
				}
,
				"obj-4::obj-27::obj-3::obj-99" : 				{
					"parameter_longname" : "clip[2]"
				}

			}
,
			"inherited_shortname" : 1
		}
,
		"dependency_cache" : [ 			{
				"name" : "ierf.gendsp",
				"bootpath" : "~/Documents/Max 8/Library/latent_control",
				"patcherrelativepath" : ".",
				"type" : "gDSP",
				"implicit" : 1
			}
, 			{
				"name" : "latent_remote.js",
				"bootpath" : "~/Documents/Max 8/Library/latent_control",
				"patcherrelativepath" : ".",
				"type" : "TEXT",
				"implicit" : 1
			}
, 			{
				"name" : "latent_remote.maxpat",
				"bootpath" : "~/Documents/Max 8/Library/latent_control",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "latent_slider.maxpat",
				"bootpath" : "~/Documents/Max 8/Library/latent_control",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "latent_slider_component.maxpat",
				"bootpath" : "~/Documents/Max 8/Library/latent_control",
				"patcherrelativepath" : ".",
				"type" : "JSON",
				"implicit" : 1
			}
, 			{
				"name" : "thru.maxpat",
				"bootpath" : "C74:/patchers/m4l/Pluggo for Live resources/patches",
				"type" : "JSON",
				"implicit" : 1
			}
 ],
		"autosave" : 0
	}

}
