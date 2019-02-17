class CfgVehicles
{
	class Container_Base;
	class Inventory_Base;
	class FenceKit;
	class FenceKitPlacing;

	class Barrel_ColorBase : Container_Base
	{
		InteractActions[] = { 1025,1026,30001,30002 };
	};
	
	class EpochKey_Base : Inventory_Base {
		scope = 0;
		model = "\emt\dzsa_epoch_assets\emt\items\vehicle_key\vehicle_key_01.p3d";
	};
	class EpochKey_Black : EpochKey_Base {
		scope = 2;
		displayName = "Vehicle Key (Black 1234)";
		descriptionShort = "Vehicle Key (Black)";
		animClass = "Knife";
		// ContinuousActions[] = { 146 };
		rotationFlags = 17;
		// RestrainUnlockType = 0;
		weight = 4;
		itemSize[] = { 1,1 };
		fragility = 0.0099999998;
		hiddenSelectionsTextures[] = { "\emt\dzsa_epoch_assets\emt\items\vehicle_key\data\key_01_nohq.paa","\emt\dzsa_epoch_assets\DS\Masks\welding_mask_devilscamo_co.paa","\emt\dzsa_epoch_assets\DS\Masks\welding_mask_devilscamo_co.paa" };
	};

	class Armband_ColorBase;
	class Armband_EPOCH: Armband_ColorBase
	{
		scope = 2;
		author = "DirtySanchez";
		displayName = "EPOCH Armband";
		descriptionShort = "Epoch Rocks!";
		vehicleClass = "Epoch_Clothing";
		visibilityModifier = 0.85;
		color = "Epoch";
		hiddenSelectionsTextures[] = {"\emt\dzsa_epoch_assets\DS\Armbands\armbend_epoch_g_co.paa","\emt\dzsa_epoch_assets\DS\Armbands\armbend_small_epoch_co.paa","\emt\dzsa_epoch_assets\DS\Armbands\armbend_small_epoch_co.paa","\emt\dzsa_epoch_assets\DS\Armbands\armbend_big_epoch_co.paa","\emt\dzsa_epoch_assets\DS\Armbands\armbend_big_epoch_co.paa"};
	};
	class WeldingMask;
	class WeldingMask_DevilsCamo: WeldingMask
	{
		scope = 2;
		author = "DirtySanchez";
		displayName = "DevilsCamo Welding Mask";
		color = "DevilsCamo";
		vehicleClass = "Epoch_Headgear";
		visibilityModifier = 0.85;
		noMask = 1;
		hiddenSelectionsTextures[] = {"\emt\dzsa_epoch_assets\DS\Masks\welding_mask_devilscamo_co.paa","\emt\dzsa_epoch_assets\DS\Masks\welding_mask_devilscamo_co.paa","\emt\dzsa_epoch_assets\DS\Masks\welding_mask_devilscamo_co.paa"};

	};
	///walls
	
	class test_wall_Kit: FenceKit
	{
		scope=2;
		displayName="Test Wall Kit";
		descriptionShort="Test Wall Kit";
	};
	class test_wall_Placing: FenceKitPlacing
	{
		scope=2;
		model="\DZ\gear\camping\fence_kit_placing.p3d";
		hiddenSelections[]=
		{
			"placing"
		};
		hiddenSelectionsTextures[]=
		{
			"dz\gear\consumables\data\pile_of_planks_co.tga"
		};
		hiddenSelectionsMaterials[]=
		{
			"dz\gear\camping\data\fence_pile_of_planks.rvmat"
		};
		hologramMaterial="tent_medium";
		hologramMaterialPath="emt\dzsa_epoch_pv_assets\emt\baseb\walls\data";/// change this
		slopeLimit=0.25;
	};
	class test_Wall: Inventory_Base
	{
		scope = 2;
		displayName = "Test_Wall";
		descriptionShort = "Test_Wall";
		model = "\emt\dzsa_epoch_pv_assets\emt\baseb\walls\wall_1m.p3d";
		bounding = "BSphere";
		overrideDrawArea = "3.0";
		forceFarBubble = "true";
		SingleUseActions[] = {527};
		ContinuousActions[] = {155,239};
		InteractActions[] = {1051};
		armAction = "Disarm";
		handheld = "false";
		lootCategory = "Crafted";
		carveNavmesh = 1;
		weight = 3000;
		itemSize[] = {2,3};
		useEntityHierarchy = "true";
		physLayer = "item_large";
		rotationFlags = 2;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
				};
			};
			class GlobalArmor
			{
				class Projectile
				{
					Health = 0;
					Blood = 0;
					Shock = 0;
				};
			};
		};
		attachments[] = 
			{
			"Wall_Barbedwire_1",
			"Wall_Barbedwire_2",
			//"Wall_Camonet",
			//"Att_CombinationLock",
			"Material_Nails",
			"Material_WoodenPlanks",
			"Material_MetalSheets",
			"Material_WoodenLogs",   
			"Material_MetalWire"	
			};
		class GUIInventoryAttachmentsProps
		{
			class Base
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Base";
				description="";
				attachmentSlots[]=
				{
					"Material_WoodenLogs"
				};
				icon="missing";
				selection="wall";
			};
			class Attachments
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Attachments";
				description="";
				attachmentSlots[]=
				{
					"Wall_Camonet",
					"Wall_Barbedwire_1",
					"Wall_Barbedwire_2",
					"Att_CombinationLock"
				};
				icon="missing";
				selection="wall";
			};
			class Material
			{
				name="$STR_CfgVehicles_Fence_Att_Category_Materials";
				description="";
				attachmentSlots[]=
				{
					"Material_Nails",
					"Material_WoodenPlanks",
					"Material_MetalSheets",
					"Material_MetalWire"
				};
				icon="missing";
				selection="wall";
			};
		};
		class AnimationSources
			{
				class AnimSourceShown
				{
					source = "user";
					animPeriod = 0.01;
					initPhase = 0;
				};
				class AnimSourceHidden
				{
					source = "user";
					animPeriod = 0.01;
					initPhase = 1;
				};
				class AnimRotate
				{
					source = "user";
					animPeriod = 0.01;
					initPhase = 0;
				};
				class Wall_Interact_Rotate: AnimRotate{};
				class Wall_Barbedwire_Mounted_Rotate: AnimRotate{};
				class Wall_Base_Down_Rotate: AnimRotate{};
				class Wall_Base_Up_Rotate: AnimRotate{};
				class material_rockpile: AnimSourceHidden{};
				class material_bucket: AnimSourceHidden{};
				class Wall_Barbedwire_1: AnimSourceHidden{};
				class Wall_Barbedwire_1_Mounted: AnimSourceHidden{};
				class Wall_Barbedwire_2: AnimSourceHidden{};
				class Wall_Barbedwire_2_Mounted: AnimSourceHidden{};
				class Deployed: AnimSourceHidden{};
				class Base: AnimSourceHidden{};
				class Wall_Platform: AnimSourceHidden{};
				class Wall_Platform_Stairs_Left: AnimSourceHidden{};
				class Wall_Platform_Stairs_Right: AnimSourceHidden{};
				class Wall_Base_Down: AnimSourceHidden{};
				class Wall_Base_Up: AnimSourceHidden{};
			};
		class Construction
		{
			class wall
			{
				class base
				{
					name="$STR_CfgVehicles_Construction_Part_Base";
					is_base=1;
					required_parts[]={};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=4;
					dismantle_action_type=4;
					material_type=1;
					class Materials
					{
						class Material1
						{
							type="WoodenLog";
							slot_name="Material_WoodenLogs";
							quantity=2;
							lockable=1;
						};
					};
				};
				class wall_platform
				{
					name="$STR_CfgVehicles_Construction_Part_Platform";
					required_parts[]=
					{
						"base"
					};
					conflicted_parts[]=
					{
						"wall_gate"
					};
					collision_data[]=
					{
						"wall_platform_min",
						"wall_platform_max"
					};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=3;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=5;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=15;
						};
					};
				};
				class wall_platform_stairs_left
				{
					name="$STR_CfgVehicles_Construction_Part_Platform_Stairs_Left";
					required_parts[]=
					{
						"wall_platform"
					};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=3;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=4;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=8;
						};
					};
				};
				class wall_platform_stairs_right
				{
					name="$STR_CfgVehicles_Construction_Part_Platform_Stairs_Right";
					required_parts[]=
					{
						"wall_platform"
					};
					conflicted_parts[]={};
					collision_data[]={};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=3;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=4;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=8;
						};
					};
				};
				class wall_gate
				{
					name="$STR_CfgVehicles_Construction_Part_Gate";
					is_gate=1;
					required_parts[]=
					{
						"base",
						"wall_base_down",
						"wall_base_up"
					};
					conflicted_parts[]=
					{
						"wall_platform"
					};
					collision_data[]={};
					build_action_type=1;
					dismantle_action_type=1;
					material_type=5;
					class Materials
					{
						class Material1
						{
							type="MetalWire";
							slot_name="Material_MetalWire";
							quantity=-1;
							lockable=1;
						};
					};
				};
				class wall_base_down
				{
					name="$STR_CfgVehicles_Construction_Part_Base_Down";
					required_parts[]=
					{
						"base"
					};
					conflicted_parts[]={};
					collision_data[]=
					{
						"wall_down_min",
						"wall_down_max"
					};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=2;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=4;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=8;
						};
					};
				};
				class wall_wood_down
				{
					name="$STR_CfgVehicles_Construction_Part_Wood_Down";
					required_parts[]=
					{
						"wall_base_down"
					};
					conflicted_parts[]=
					{
						"wall_metal_down"
					};
					collision_data[]=
					{
						"wall_down_min",
						"wall_down_max"
					};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=2;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=5;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=10;
						};
					};
				};
				class wall_metal_down
				{
					name="$STR_CfgVehicles_Construction_Part_Metal_Down";
					required_parts[]=
					{
						"wall_base_down"
					};
					conflicted_parts[]=
					{
						"wall_wood_down"
					};
					collision_data[]=
					{
						"wall_down_min",
						"wall_down_max"
					};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=4;
					class Materials
					{
						class Material1
						{
							type="MetalPlate";
							slot_name="Material_MetalSheets";
							quantity=3;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=12;
						};
					};
				};
				class wall_base_up
				{
					name="$STR_CfgVehicles_Construction_Part_Base_Up";
					required_parts[]=
					{
						"base"
					};
					conflicted_parts[]={};
					collision_data[]=
					{
						"wall_up_min",
						"wall_up_max"
					};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=2;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=4;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=8;
						};
					};
				};
				class wall_wood_up
				{
					name="$STR_CfgVehicles_Construction_Part_Wood_Up";
					required_parts[]=
					{
						"wall_base_up"
					};
					conflicted_parts[]=
					{
						"wall_metal_up"
					};
					collision_data[]=
					{
						"wall_up_min",
						"wall_up_max"
					};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=2;
					class Materials
					{
						class Material1
						{
							type="WoodenPlank";
							slot_name="Material_WoodenPlanks";
							quantity=5;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=10;
						};
					};
				};
				class wall_metal_up
				{
					name="$STR_CfgVehicles_Construction_Part_Metal_Up";
					required_parts[]=
					{
						"wall_base_up"
					};
					conflicted_parts[]=
					{
						"wall_wood_up"
					};
					collision_data[]=
					{
						"wall_up_min",
						"wall_up_max"
					};
					build_action_type=2;
					dismantle_action_type=2;
					material_type=4;
					class Materials
					{
						class Material1
						{
							type="MetalPlate";
							slot_name="Material_MetalSheets";
							quantity=3;
						};
						class Material2
						{
							type="Nail";
							slot_name="Material_Nails";
							quantity=12;
						};
					};
				};
			};
		};
			class PointLights{};

	};


};