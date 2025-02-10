using MelonLoader;
using UnityEngine;
using HarmonyLib;
using Il2Cpp;
using System.Runtime.CompilerServices;
using System.Net;

namespace devourUnlocker
{

    [HarmonyLib.HarmonyPatch(typeof(SurvivalLobbyController), "KickPlayer")]
    class antiKick
    {
        static bool Prefix(int index)
        {
            Melon<Unlocker>.Logger.Msg($"tried kicking: {index}");
            return false;
        }
    }
    [HarmonyLib.HarmonyPatch(typeof(NetworkCredential), "set_Password")]
    class getPassword
    {
        static void Postfix(string __result)
        {
            Melon<Unlocker>.Logger.Msg($"password: {__result}");
        }
    }

    public class Unlocker : MelonMod
    {
        public override void OnLateInitializeMelon()
        {
            var patches = Harmony.GetPatchedMethods();
            MelonLogger.Msg("patches:");
            foreach (var patch in patches)
            {
                LoggerInstance.Msg(patch.Name);
            }

        }


        public override void OnSceneWasInitialized(int buildIndex, string sceneName)
        {
            LoggerInstance.Msg($"{sceneName} initialized");
        }
        public override void OnUpdate()
        {
            if (Input.GetKeyDown(KeyCode.F1)) 
            {
                OptionsHelpers[] helper = GameObject.FindObjectsOfType<OptionsHelpers>();
                foreach (OptionsHelpers helper2 in helper)
                {
                    foreach(CharacterOutfit fit in helper2.outfits)
                    {
                        fit.isHidden = false;
                        fit.isOwned = true;
                        
                    }
                    LoggerInstance.Msg("unlocked fits");
                    foreach (CharacterPet fit in helper2.pets)
                    {
                        fit.isHidden = false;
                        fit.isOwned = true;

                    }
                    LoggerInstance.Msg("unlocked pets");
                    foreach (CharacterPerk fit in helper2.perks)
                    {
                        fit.isHidden = false;
                        fit.cost = -6942;
                    }
                    LoggerInstance.Msg("perk cost setter");
                    foreach (CharacterFlashlight fit in helper2.flashlights)
                    {
                        fit.isHidden =  false;
                        if (fit.isSupporter)
                        {
                            fit.isOwned = true;
                        }
                        else
                        {
                            if (fit.cost != 0)
                            {
                                fit.cost = -6666;
                            }
                            else
                            {
                                fit.isOwned = true;
                            }
                        }
                        

                    }
                    LoggerInstance.Msg("flashlight unlocker");
                    foreach (CharacterEmote fit in helper2.emotes)
                    {
                        fit.isHidden = false;
                        fit.isOwned = true;
                    }
                    LoggerInstance.Msg("emote unlocked");



                }

            }
        }

    }
}