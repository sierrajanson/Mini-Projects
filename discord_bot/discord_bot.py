# made by sierra janson
# ty discord documentation devs: https://discordpy.readthedocs.io/en/stable/ext/commands/commands.html
# ty random mp3 example file downloader: 
# ty stackoverflow: https://stackoverflow.com/questions/55284892/discord-bot-python-discord-errors-clientexception-ffmpeg-was-not-found
# ty detailed ffmpeg install tutorial: https://www.editframe.com/guides/how-to-install-and-start-using-ffmpeg-in-under-10-minutes
# you have to install & add ffmpeg to PATH, follow above tutorial ^
# ty stackoverflow again: https://stackoverflow.com/questions/44868656/vs-code-terminal-doesnt-recognize-path-variables

import discord
from discord.ext import commands

intents = discord.Intents.default()
intents.message_content = True
bot = commands.Bot(command_prefix='$', intents=intents)

def replay_sound(ctx, sound_name,end_condition):
    if end_condition > 0:
        if ctx.voice_client:
            source = discord.FFmpegPCMAudio(sound_name)
            ctx.voice_client.play(source, after=lambda e: replay_sound(ctx, sound_name,end_condition-1))
    else:
        ctx.voice_client.disconnect()


def file_exists(path):
    import os
    return os.path.isfile(path)

@bot.command(name='anti_eep', help='help my friends stay awake')
async def join(ctx, repeat_times=2):
    if ctx.author.voice:
        channel = ctx.author.voice.channel
        await channel.connect()
        if ctx.voice_client:
            song_path = "chippi.mp3"
            assert(file_exists(song_path)) # download the file if you haven't 
            source = discord.FFmpegPCMAudio(song_path)
            ctx.voice_client.play(source, after=lambda e: replay_sound(ctx,song_path,int(repeat_times)-1))
    else:
        await ctx.send("join a vc")

@bot.command(name='go_away', help='eepy time')
async def leave(ctx):
    if ctx.voice_client:
        await ctx.voice_client.disconnect()
    else:
        await ctx.send("i'm not in vc goober")




token = ""
assert(token != "") # get your own discord developer token nerd
bot.run(token)

