modded class PluginRecipesManagerBase extends PluginBase
{
	override void RegisterRecipies()
	{
		RegisterRecipe(new Craft_test_wall_Kit);
		super.RegisterRecipies();
	}
};