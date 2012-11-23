/*=========================================================================

  Program:   Vector Image Scaling Class for ITK.
  Language:  C++

  Copyright (c) Brigham and Women's Hospital. All rights reserved.
  See ITKCopyright.txt or http://www.itk.org/HTML/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even 
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR 
     PURPOSE.  See the above copyright notices for more information.

=========================================================================*/
#ifndef __itkScaleVectorImageFilter_txx
#define __itkScaleVectorImageFilter_txx

#include "itkScaleVectorImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"
#include "itkImageRegionIterator.h"
#include "itkImageRegionConstIterator.h"
#include "itkListSample.h"
#include "itkCrossHelper.h"

#include "vnl/vnl_math.h"

namespace itk
{

/**
 * Constructor
 */
template < typename  TInput, typename TOutput  >
ScaleVectorImageFilter< TInput, TOutput >
::ScaleVectorImageFilter()
{
}


template < typename  TInput, typename TOutput  >
void 
ScaleVectorImageFilter< TInput, TOutput >
::GenerateData()
{
  itkDebugMacro(<< "ScaleVectorImageFilter generating data ");
  
  typename InputImageType::ConstPointer input = this->GetInput();
  typename OutputImageType::Pointer output = this->GetOutput();

  ImageRegionConstIterator<InputImageType> it;
  it = ImageRegionConstIterator<InputImageType>( input, input->GetRequestedRegion() );
  ImageRegionIterator<OutputImageType> oit;
  this->AllocateOutputs();
  oit = ImageRegionIterator<OutputImageType>(output,
                                             output->GetRequestedRegion());

  std::cout << "scale = ("
            << this->m_ScaleVector[0] << ", "
            << this->m_ScaleVector[1] << ", "
            << this->m_ScaleVector[2] << ")" << std::endl;
    

  output->FillBuffer(static_cast<OutputPixelType>(0));
  
  //-- Create lists of points for each label
  oit.GoToBegin();
  it.GoToBegin();
  while (!it.IsAtEnd())
    {
    InputPixelType pix = it.Get();
    OutputPixelType opix = oit.Get();
    //typename InputImageType::IndexType index = it.GetIndex();
    opix = 0.0;
    for (int i = 0; i < InputImageType::ImageDimension; i ++)
      {
      double p = pix[i] * this->m_ScaleVector[i];
      opix += p*p;
      }
    opix = sqrt(opix);
    oit.Set(opix);
    ++it;
    ++oit;
    }
    
}


template < typename  TInput, typename TOutput  >
void
ScaleVectorImageFilter< TInput, TOutput >
::PrintSelf(std::ostream& os, Indent indent) const
{
  Superclass::PrintSelf(os, indent);
}


} // end namespace itk
  
#endif
