from setuptools import setup

setup(
    name='connect-4',
    version='1.0',
    packages=['connect_4'],
    url='',
    license='',
    author='SamProkopchuk',
    author_email='ssprokopchuk@gmail.com',
    description='An environment to test various Connect-4 AIs',
    install_requires=['numpy>=1.18.5'],
    classifiers=[
        'Programming Language :: Python :: 3',
        'License :: OSI Approved :: MIT License',
        'Operating System :: OS Independent',
    ],
    python_requires='>=3.8',
)
